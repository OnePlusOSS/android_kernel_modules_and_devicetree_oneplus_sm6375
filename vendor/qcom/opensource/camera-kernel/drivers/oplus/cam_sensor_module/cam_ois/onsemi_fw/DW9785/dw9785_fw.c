/**
  ******************************************************************************
  * File Name          : DW9785_SET_API.C
  * Description        : Main program c file
  * DongWoon Anatech   :
  * Version            : 0.1
  ******************************************************************************
  *
  * COPYRIGHT(c) 2023 DWANATECH
  * DW9785 Setup Program for SET
  * Revision History
  * 2023.10.25 by JH Seo
  *            - Drift
  ******************************************************************************
**/



//#include <stdio.h>
//#include "stdafx.h"
//#include <stdarg.h>
//#include <wchar.h>
//#include "math.h"
//#include "mmsystem.h"
//#include "string.h"

//#include "func.h"

#include "DW9785_Cadillac_FW_T0.h"
#include "DW9785_Cadillac_FW_EVT.h"
#include <linux/types.h>
#include "cam_sensor_util.h"
#include "cam_debug_util.h"
#include "fw_download_interface.h"
#include "dw9785_fw.h"
typedef int(*ptr_func)(const char*, ...);
/*
#else
#include "main.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "dw9785 set api rev0.1.h"
#include "DW9785_Cadillac_FW_V0101_D1018.h"

extern unsigned char DW9785_ID;

int debug_printf(char *buf, ...);
typedef int(*ptr_func)(const char*, ...);
ptr_func logi = debug_printf;
void HAL_Delay(unsigned int Delay);
int debug_printf(char *buf, ...)
{
	int len = 0;
	char pBuf[512];
	va_list ap;
	va_start(ap, buf);
	len = vsprintf(pBuf, buf, ap);
	pBuf[len] = '\r';
	pBuf[len + 1] = '\n';
	pBuf[len + 2] = '\0';
	printf(pBuf, ap);
	va_end(ap);
	return 0;
}
#endif
*/

/*Global buffer for flash download*/
unsigned short g_updateFw;
uint32_t DW9785_FW_VERSION;
uint32_t DW9785_FW_DATE;
/* global array for ois aging test */
uint32_t g_get_actual[OIS_AGING_LOOP_DATA_NUM];
uint32_t g_loop_done;

/*void mdelay(int delay)
{
	//HAL_Delay(count);

	DWORD msec = delay;
	DWORD dwStart;

	if (msec == 0){
		msec = 1;
	}

	//Sleep(msec);

	timeBeginPeriod(2);
	dwStart = timeGetTime();
	while(1){
		// counter return zero when the system continues to work for 49.7 days
		if((timeGetTime() - dwStart) >= msec){
			break;
		}
	}
	timeEndPeriod(2);
}*/

int dw9785_download_open_camera(struct cam_ois_ctrl_t *o_ctrl)
{
	int ret = EOK;
	int err_Ready = 0;
	//unsigned short fw_version_current = 0;
	//unsigned short fw_type = 0;
	//int store_flag = 0;
	err_Ready = dw9785_ready_check(o_ctrl);
	if (err_Ready == ERROR_PRODUCTID)
	{
		CAM_ERR(CAM_OIS,"[dw9785_download_open_camera] dw9785_ready_check result : failed to check the product_id(0x9785)");
		CAM_ERR(CAM_OIS,"[dw9785_download_open_camera] stop the dw9785 ic boot operation");
		dw9785_deep_sleep(o_ctrl);
		return ERROR_PRODUCTID;
	}else if (err_Ready == ERROR_FLASH_CHECKSUM)
	{
		/* it works when the function is enabled in dw9785_ready_check() */
		g_updateFw = 1;
		CAM_ERR(CAM_OIS,"[dw9785_download_open_camera] dw9785_ready_check result : The firmware checksum check of the flash memory failed.");
		CAM_ERR(CAM_OIS,"[dw9785_download_open_camera] force to recovery firmware");
	}else if (err_Ready == ERROR_RV_CHECKSUM)
	{
		/* it works when the function is enabled in dw9785_ready_check() */
		g_updateFw = 1;
		CAM_ERR(CAM_OIS,"[dw9785_download_open_camera] dw9785_ready_check result : rv checksum failed");
		CAM_ERR(CAM_OIS,"[dw9785_download_open_camera] force to recovery firmware");
	}else if (err_Ready == ERROR_FW_VALID)
	{
		/* it works when the function is enabled in dw9785_ready_check() */
		g_updateFw = 1;
		CAM_ERR(CAM_OIS,"[dw9785_download_open_camera] dw9785_ready_check result : firmware version check failed");
		CAM_ERR(CAM_OIS,"[dw9785_download_open_camera] update to latest firmware");
	}

	if (g_updateFw)
	{
		CAM_ERR(CAM_OIS,"[dw9785_download_open_camera] start downloading the latest version firmware, ver: 0x%04X", DW9785_FW_VERSION);
		CAM_ERR(CAM_OIS,"[dw9785_download_open_camera] fw download packet size : %d", PKT_SIZE);
		if(dw9785_download_fw(o_ctrl) == EOK)
		{
			/* fw download success */
			CAM_ERR(CAM_OIS,"[dw9785_download_open_camera] complete fw download");
		}else{
			CAM_ERR(CAM_OIS,"[dw9785_download_open_camera] 1st fw download failed, retry 2nd fw download");
			dw9785_device_reset(o_ctrl);
			if(dw9785_download_fw(o_ctrl) == EOK)
			{
				/* fw download success */
				CAM_ERR(CAM_OIS,"[dw9785_download_open_camera] complete 2nd fw download");
			}else{
				/* fw download failed */
				dw9785_deep_sleep(o_ctrl);
				CAM_ERR(CAM_OIS,"[dw9785_download_open_camera] 2nd fw download failed, enter ic deep sleep mode");
				return ERROR_FW_DOWN_FAIL;
			}
		}
	}
	dw9785_ois_reset(o_ctrl);
	CAM_ERR(CAM_OIS,"[dw9785_download_open_camera] finish");
	return ret;
}

int dw9785_download_fw_reply(struct cam_ois_ctrl_t *o_ctrl)
{
	uint32_t FMC;
	unsigned short* DW9785_FW = NULL;
	DW9785_FW = DW9785_FW_EVT;
	CAM_ERR(CAM_OIS,"[dw9785_download_fw_reply] start");
	/* step 1: MCU Disable (sleep mode)*/
	dw9785_mcu_disable(o_ctrl);
	dw9785_wakeup(o_ctrl);

	/* step 2: prog mem protection off */
	dw9785_prog_pt_off(o_ctrl);

	/* step 3: erase flash fw area */
	if(dw9785_fw_eflash_erase(o_ctrl) != EOK)	return ERROR_FW_ERASE;

	/* step 4: FMC register check */
	write_reg_16bit_value_16bit(o_ctrl,0xDE01, PROG_MEM_SEL); /* FMC block FW select */
	mdelay(1);
	read_reg_16bit_value_16bit(o_ctrl,0xDE01, &FMC);
	if (FMC != PROG_MEM_SEL)
	{
		CAM_ERR(CAM_OIS,"[dw9785_download_fw_reply] fmc register value 1st warning : %04x", FMC);
		write_reg_16bit_value_16bit(o_ctrl,0xDE01, PROG_MEM_SEL);
		mdelay(1);
		FMC = 0;

		read_reg_16bit_value_16bit(o_ctrl,0xDE01, &FMC);
		if (FMC != 0)
		{
			CAM_ERR(CAM_OIS,"[dw9785_download_fw_reply] 2nd fmc register value 2nd warning : %04x", FMC);
			CAM_ERR(CAM_OIS,"[dw9785_download_fw_reply] stop f/w download");
			return ERROR_FW_DOWN_FMC;
		}
	}

	/* step 5: firmware sequential write to flash */
	/* updates the module status before firmware download */
	CAM_ERR(CAM_OIS,"[dw9785_download_fw_reply] fw ver. : 0x%04x, fw size : %d [Byte]", DW9785_FW_VERSION, DW9785_MCS_SIZE_W);
	CAM_ERR(CAM_OIS,"[dw9785_download_fw_reply] sequential write start");
	for (int i = 0; i < DW9785_MCS_SIZE_W/2 ; i += PKT_SIZE) //DW9785_MCS_SIZE_W==32256, PKT_SIZE == 16
	{
		int32_t rc = 0;
		rc = write_reg_16bit_value_16bit(o_ctrl,0xDE04, DW9785_MCS_START_ADDRESS + i*2); // DW9785_MCS_START_ADDRESS == 0x0000
		if (rc < 0) {
			CAM_ERR(CAM_OIS, "write_reg_16bit_value_16bit failed");
			return rc;
		} else {
			rc = dw9785_i2c_block_write_reg(o_ctrl, 0xDE05, (unsigned short*)(DW9785_FW + i), PKT_SIZE);
			if (rc < 0) {
				CAM_ERR(CAM_OIS, "dw9785_i2c_block_write_reg failed");
				return rc;
			}
		}
	}
	mdelay(300);
	if(dw9785_wait_check_register(o_ctrl, 0xDE00, 0x0000, LOOP, DW9785_WAIT_TIME) == EOK)	CAM_ERR(CAM_OIS,"[dw9785_download_fw_reply] sequential write done");

	/* step 6: memory load from sram */
	dw9785_ois_reset(o_ctrl);

	dw9785_servo_on(o_ctrl);
	write_reg_16bit_value_16bit(o_ctrl,0x7710, 0x000E);
	dw9785_set_cal_store(o_ctrl);
    return EOK;
}
int dw9785_download_fw(struct cam_ois_ctrl_t *o_ctrl)
{
	unsigned char ret = EOK;
	uint32_t FMC;
	uint32_t fw_version_current;
	uint32_t fw_data_current;
	uint32_t act_id_current;
	unsigned short* DW9785_FW = NULL;
	int rc = -1;
	uint32_t reg_flash_checksum_status; /* 0xD010 */
	uint32_t reg_rv_checksum_status; /* 0xD011 */

	DW9785_FW_VERSION = 0;
	DW9785_FW_DATE = 0;

	/* check module info & fw version*/
	rc = dw9785_ois_reset(o_ctrl);
	if(rc != FUNC_PASS){
		CAM_ERR(CAM_OIS,"[dw9785_download_fw] reset failed...");
		return rc;
	}

	read_reg_16bit_value_16bit(o_ctrl, 0x7001, &fw_version_current);
	read_reg_16bit_value_16bit(o_ctrl, 0x7002, &fw_data_current);
	read_reg_16bit_value_16bit(o_ctrl, 0x74FA, &act_id_current);
	read_reg_16bit_value_16bit(o_ctrl,0xD010, &reg_flash_checksum_status);
	read_reg_16bit_value_16bit(o_ctrl,0xD011, &reg_rv_checksum_status);

	CAM_ERR(CAM_OIS,"[dw9785_download_fw] module info, ver:[0x%.4x] date:[0x%.4x] act_id:[0x%.4x]", fw_version_current, fw_data_current, act_id_current);

	if ((fw_version_current < DW9785_FW_VERSION_EVT) || (reg_rv_checksum_status != DW9785_AUTORD_RV_OK) ||
	(reg_flash_checksum_status != FLASH_CHECKSUM_OK)) {
		DW9785_FW_VERSION = DW9785_FW_VERSION_EVT;
		DW9785_FW_DATE = DW9785_FW_DATE_EVT;
		DW9785_FW = DW9785_FW_EVT;
	} else {
		CAM_ERR(CAM_OIS,"[dw9785_download_fw] This is the latest version!");
		return FUNC_FAIL;
	}

	CAM_ERR(CAM_OIS,"[dw9785_download_fw] start");
	/* step 1: MCU Disable (sleep mode)*/
	dw9785_mcu_disable(o_ctrl);
	dw9785_wakeup(o_ctrl);

	/* step 2: prog mem protection off */
	dw9785_prog_pt_off(o_ctrl);

	/* step 3: erase flash fw area */
	if(dw9785_fw_eflash_erase(o_ctrl) != EOK)	return ERROR_FW_ERASE;

	/* step 4: FMC register check */
	write_reg_16bit_value_16bit(o_ctrl,0xDE01, PROG_MEM_SEL); /* FMC block FW select */
	mdelay(1);
	read_reg_16bit_value_16bit(o_ctrl,0xDE01, &FMC);
	if (FMC != PROG_MEM_SEL)
	{
		CAM_ERR(CAM_OIS,"[dw9785_download_fw] fmc register value 1st warning : %04x", FMC);
		write_reg_16bit_value_16bit(o_ctrl,0xDE01, PROG_MEM_SEL);
		mdelay(1);
		FMC = 0;

		read_reg_16bit_value_16bit(o_ctrl,0xDE01, &FMC);
		if (FMC != 0)
		{
			CAM_ERR(CAM_OIS,"[dw9785_download_fw] 2nd fmc register value 2nd warning : %04x", FMC);
			CAM_ERR(CAM_OIS,"[dw9785_download_fw] stop f/w download");
			return ERROR_FW_DOWN_FMC;
		}
	}

	/* step 5: firmware sequential write to flash */
	/* updates the module status before firmware download */
	CAM_ERR(CAM_OIS,"[dw9785_download_fw] fw ver. : 0x%04x, fw size : %d [Byte]", DW9785_FW_VERSION, DW9785_MCS_SIZE_W);
	CAM_ERR(CAM_OIS,"[dw9785_download_fw] sequential write start");
	for (int i = 0; i < DW9785_MCS_SIZE_W/2 ; i += PKT_SIZE) //DW9785_MCS_SIZE_W==32256, PKT_SIZE == 16
	{
		int32_t rc = 0;
		rc = write_reg_16bit_value_16bit(o_ctrl,0xDE04, DW9785_MCS_START_ADDRESS + i*2); // DW9785_MCS_START_ADDRESS == 0x0000
		if (rc < 0) {
			CAM_ERR(CAM_OIS, "write_reg_16bit_value_16bit failed");
			return rc;
		} else {
			rc = dw9785_i2c_block_write_reg(o_ctrl, 0xDE05, (unsigned short*)(DW9785_FW + i), PKT_SIZE);
			if (rc < 0) {
				CAM_ERR(CAM_OIS, "dw9785_i2c_block_write_reg failed");
				return rc;
			}
		}
	}
	mdelay(300);
	if(dw9785_wait_check_register(o_ctrl, 0xDE00, 0x0000, LOOP, DW9785_WAIT_TIME) == EOK)	CAM_ERR(CAM_OIS,"[dw9785_download_fw] sequential write done");

	/* step 6: memory load from sram */
	dw9785_ois_reset(o_ctrl);

	dw9785_servo_on(o_ctrl);
	write_reg_16bit_value_16bit(o_ctrl,0x7710, 0x000E);
	dw9785_set_cal_store(o_ctrl);

	/* step 7: check fw_checksum */
	if(dw9785_fw_checksum_check(o_ctrl) == EOK)
	{
		CAM_ERR(CAM_OIS,"[dw9785_download_fw] fw download success.");
		CAM_ERR(CAM_OIS,"[dw9785_download_fw] finish");
	} else
	{
		CAM_ERR(CAM_OIS,"[dw9785_download_fw] fw download cheksum fail.");
		CAM_ERR(CAM_OIS,"[dw9785_download_fw] finish");
		for (int i = 0; i < 3; i++)
		{
			dw9785_download_fw_reply(o_ctrl);
			if(dw9785_fw_checksum_check(o_ctrl) == EOK)
			{
				ret = EOK;
				CAM_ERR(CAM_OIS,"[dw9785_download_fw_reply Ok] Ok Cnt %d", i);
				return ret;
			}
			CAM_ERR(CAM_OIS,"[dw9785_download_fw_reply] Cnt %d", i);
		}
		ret = ERROR_FW_CHECKSUM;
	}
	return ret;
}

int dw9785_ready_check(struct cam_ois_ctrl_t *o_ctrl)
{
	uint32_t reg_flash_checksum_status; /* 0xD011 */
	uint32_t reg_rv_checksum_status; /* 0xD010 */
	unsigned short fw_version_current;

	CAM_ERR(CAM_OIS,"[dw9785_ready_check] start");

	dw9785_ois_reset(o_ctrl);

	/* check product id*/
	if(dw9785_product_id_check(o_ctrl) != DW9785_CHIP_ID){
		CAM_ERR(CAM_OIS,"[dw9785_ready_check] product id - error");
		CAM_ERR(CAM_OIS,"[dw9785_ready_check] finish");
		return ERROR_PRODUCTID;
	}
	CAM_ERR(CAM_OIS,"[dw9785_ready_check] product id : 0x9785 - pass");

	/* steps to check fw, param, x_data checksum */
	read_reg_16bit_value_16bit(o_ctrl,0xD010, &reg_flash_checksum_status);
	if(!((reg_flash_checksum_status & FLASH_FW_CHECKSUM_OK) == FLASH_FW_CHECKSUM_OK)){
		CAM_ERR(CAM_OIS,"[dw9785_ready_check] fw flash checksum : 0x%04x - error (0x%04x != 0x%04x)", reg_flash_checksum_status,reg_flash_checksum_status & FLASH_FW_CHECKSUM_OK, FLASH_FW_CHECKSUM_OK);
		CAM_ERR(CAM_OIS,"[dw9785_ready_check] finish");
		return ERROR_FLASH_CHECKSUM;
	}
	CAM_ERR(CAM_OIS,"[dw9785_ready_check] flash checksum : 0x%04x - pass", reg_flash_checksum_status);

	/* steps to check whether auto_read is working */
	read_reg_16bit_value_16bit(o_ctrl,0xD011, &reg_rv_checksum_status);
	if(reg_rv_checksum_status != DW9785_AUTORD_RV_OK){
		CAM_ERR(CAM_OIS,"[dw9785_ready_check] rv checksum : 0x%04x - fail", reg_rv_checksum_status);
		CAM_ERR(CAM_OIS,"[dw9785_ready_check] finish");
		return ERROR_RV_CHECKSUM;
	}
	CAM_ERR(CAM_OIS,"[dw9785_ready_check] rv checksum : 0x%04x - pass", reg_rv_checksum_status);
	fw_version_current = dw9785_fw_ver_check(o_ctrl);
	if(fw_version_current != DW9785_FW_VERSION){
		CAM_ERR(CAM_OIS,"[dw9785_ready_check] update fw to the latest version (latest-fw ver: 0x%04X, date ver: 0x%04X)", DW9785_FW_VERSION, DW9785_FW_DATE);
		CAM_ERR(CAM_OIS,"[dw9785_ready_check] finish");
		return ERROR_FW_VALID;
	}else{
		CAM_ERR(CAM_OIS,"[dw9785_ready_check] the firmware version is up to date, ver: 0x%04X", DW9785_FW_VERSION);
	}
	CAM_ERR(CAM_OIS,"[dw9785_ready_check] finish");
	return EOK;
}

unsigned short dw9785_product_id_check(struct cam_ois_ctrl_t *o_ctrl)
{
	uint32_t product_id;
	read_reg_16bit_value_16bit(o_ctrl,0xD004, &product_id); /* Product ID can be confirmed by reading address 0xD016. */
	CAM_ERR(CAM_OIS,"[dw9785_product_id_check] product_id(0x%04X) : 0x%04X", 0xD004, product_id);
	return product_id;
}

unsigned short dw9785_chip_id_check(struct cam_ois_ctrl_t *o_ctrl)
{
	uint32_t chip_id;
	read_reg_16bit_value_16bit(o_ctrl,0x7000, &chip_id);
	CAM_ERR(CAM_OIS,"[dw9785_chip_id_check] chip_id(0x7000) : 0x%04X", chip_id);
	return chip_id;
}

unsigned short dw9785_fw_ver_check(struct cam_ois_ctrl_t *o_ctrl)
{
	uint32_t fw_ver;
	uint32_t fw_date;
	read_reg_16bit_value_16bit(o_ctrl,0x7001, &fw_ver);
	read_reg_16bit_value_16bit(o_ctrl,0x7002, &fw_date);
	CAM_ERR(CAM_OIS,"[dw9785_fw_ver_check] fw version : 0x%04X", fw_ver);
	CAM_ERR(CAM_OIS,"[dw9785_fw_ver_check] fw date : 0x%04X", fw_date);
	return fw_ver;
}

int dw9785_fw_type(struct cam_ois_ctrl_t *o_ctrl)
{
	uint32_t r_data;
	unsigned short eis; /* vsync(0) or qtime(1) */
	unsigned short fw_type; /* module(0) or set(1) */

	read_reg_16bit_value_16bit(o_ctrl,0x7006, &r_data);
	CAM_ERR(CAM_OIS,"[dw9785_fw_type] fw type(0x7006) : 0x%04X", r_data);
	eis = (r_data >> 8) & 0x1;
	fw_type = r_data & 0x1;

	if ( eis == EIS_VSYNC )
	{
		CAM_ERR(CAM_OIS,"[dw9785_fw_type] eis mode : vsync");
	}else if ( eis == EIS_QTIME )
	{
		CAM_ERR(CAM_OIS,"[dw9785_fw_type] eis mode : qtime");
	}

	if ( fw_type == MODULE_FW )
	{
		CAM_ERR(CAM_OIS,"[dw9785_fw_type] fw type : module fw");
	}else if ( fw_type == SET_FW )
	{
		CAM_ERR(CAM_OIS,"[dw9785_fw_type] fw type : set fw");
	}

	return fw_type;
}

void dw9785_fw_info(struct cam_ois_ctrl_t *o_ctrl)
{
	uint32_t r_data;
	CAM_ERR(CAM_OIS,"[dw9785_fw_info] start");

	read_reg_16bit_value_16bit(o_ctrl,0x7000, &r_data);
	CAM_ERR(CAM_OIS,"[dw9785_fw_info] chip_id : 0x%04X", r_data);
	read_reg_16bit_value_16bit(o_ctrl,0x7001, &r_data);
	CAM_ERR(CAM_OIS,"[dw9785_fw_info] fw version : 0x%04X", r_data);
	read_reg_16bit_value_16bit(o_ctrl,0x7002, &r_data);
	CAM_ERR(CAM_OIS,"[dw9785_fw_info] fw_date : 0x%04X", r_data);
	read_reg_16bit_value_16bit(o_ctrl,0x7003, &r_data);
	CAM_ERR(CAM_OIS,"[dw9785_fw_info] set & project : 0x%04X", r_data);
	read_reg_16bit_value_16bit(o_ctrl,0x7006, &r_data);
	CAM_ERR(CAM_OIS,"[dw9785_fw_info] vsync/qtime & set_module_fw info : 0x%04X", r_data);

	read_reg_16bit_value_16bit(o_ctrl,0x74F9, &r_data);
	CAM_ERR(CAM_OIS,"[dw9785_fw_info] module_id: 0x%04X", r_data);
	read_reg_16bit_value_16bit(o_ctrl,0x74FA, &r_data);
	CAM_ERR(CAM_OIS,"[dw9785_fw_info] actuator_id : 0x%04X", r_data);

	read_reg_16bit_value_16bit(o_ctrl,0x74FE, &r_data);
	//CAM_ERR(CAM_OIS,"[dw9785_fw_info] reg_fw_checksum : 0x%04X%04X", r_data[1], r_data[0]);
	CAM_ERR(CAM_OIS,"[dw9785_fw_info] reg_fw_checksum : 0x%02X [MSB], 0x%02X [LSB]", r_data >> 8, r_data & 0xff);
	read_reg_16bit_value_16bit(o_ctrl,0x76FE, &r_data);
	//CAM_ERR(CAM_OIS,"[dw9785_fw_info] reg_module_cal_checksum : 0x%04X%04X", r_data[1], r_data[0]);
	CAM_ERR(CAM_OIS,"[dw9785_fw_info] reg_module_cal_checksum : 0x%02X [MSB], 0x%02X [LSB]", r_data >> 8, r_data & 0xff);
	read_reg_16bit_value_16bit(o_ctrl,0x77FE, &r_data);
	//CAM_ERR(CAM_OIS,"[dw9785_fw_checksum] set-cal checksum : 0x%04X%04X", r_data[1], r_data[0]);
	CAM_ERR(CAM_OIS,"[dw9785_fw_checksum] set-cal checksum : 0x%02X [MSB], 0x%02X [LSB]", r_data >> 8, r_data & 0xff);

	read_reg_16bit_value_16bit(o_ctrl,0xD010, &r_data);
	CAM_ERR(CAM_OIS,"[dw9785_fw_info] eflash_checksum_status(0xD010) : 0x%04X", r_data);
	read_reg_16bit_value_16bit(o_ctrl,0xD011, &r_data);
	CAM_ERR(CAM_OIS,"[dw9785_fw_info] read_verification_status(0xD011) : 0x%04X", r_data);
}

void dw9785_all_pt_off(struct cam_ois_ctrl_t *o_ctrl)
{
	CAM_ERR(CAM_OIS,"[dw9785_all_pt_off]");
	write_reg_16bit_value_16bit(o_ctrl,0xDE81, 0xAD58); /* release all protection */
	mdelay(1);
}

void dw9785_prog_pt_off(struct cam_ois_ctrl_t *o_ctrl)
{
	CAM_ERR(CAM_OIS,"[dw9785_prog_pt_off]");
	write_reg_16bit_value_16bit(o_ctrl,0xDEA3, 0xAFDA); /* release prog protection */
	mdelay(1);
}

int dw9785_fw_eflash_erase(struct cam_ois_ctrl_t *o_ctrl)
{
	CAM_ERR(CAM_OIS,"[dw9785_fw_eflash_erase] start");
	CAM_ERR(CAM_OIS,"[dw9785_fw_eflash_erase] prog_auto_erase Run");
	write_reg_16bit_value_16bit(o_ctrl,0xDE06, 0x0002); /* Auto-Program Area Erase */
	mdelay(36);

	if(dw9785_wait_check_register(o_ctrl, 0xDE00, 0x0000, 3, 30) != FUNC_PASS){
		CAM_ERR(CAM_OIS,"[dw9785_fw_eflash_erase] fail");
		return ERROR_FW_ERASE;
	}
	CAM_ERR(CAM_OIS,"[dw9785_fw_eflash_erase] finish");
	return EOK;
}

void dw9785_chip_enable(struct cam_ois_ctrl_t *o_ctrl)
{
    CAM_ERR(CAM_OIS,"[dw9785_chip_enable]");
	write_reg_16bit_value_16bit(o_ctrl,0xD000, 0x0001);
	mdelay(4);
}

void dw9785_mcu_disable(struct cam_ois_ctrl_t *o_ctrl)
{
  CAM_ERR(CAM_OIS,"[dw9785_mcu_disable]");
	write_reg_16bit_value_16bit(o_ctrl,0xD001, 0x0000);
}

int dw9785_mcu_enable(struct cam_ois_ctrl_t *o_ctrl)
{
	uint32_t product_id = 0;

	read_reg_16bit_value_16bit(o_ctrl,0xD004, &product_id); /* mcu enable */
	mdelay(1); /* wait for fw initialize */
	if(product_id == DW9785_CHIP_ID){
		CAM_ERR(CAM_OIS,"[dw9785_mcu_enable] pass");
		return FUNC_PASS;
	}
		CAM_ERR(CAM_OIS,"[dw9785_mcu_enable] fail");
		return FUNC_FAIL;
}

void dw9785_wakeup(struct cam_ois_ctrl_t *o_ctrl)
{
	CAM_ERR(CAM_OIS,"[dw9785_wakeup]");
	write_reg_16bit_value_16bit(o_ctrl,0xD01A, 0x0001); /* wake up in sleep mode */
}

void dw9785_deep_sleep(struct cam_ois_ctrl_t *o_ctrl)
{
	CAM_ERR(CAM_OIS,"[dw9785_deep_sleep] : chip disable");
	write_reg_16bit_value_16bit(o_ctrl,0xD000, 0x0000); /* chip_en : disable */
	mdelay(1);
}

int dw9785_device_reset(struct cam_ois_ctrl_t *o_ctrl)
{
	int rc = FUNC_PASS;
	CAM_ERR(CAM_OIS,"[dw9785_device_reset] : sleep");
	rc = write_reg_16bit_value_16bit(o_ctrl,0xD002, 0x0001);
	if(rc != FUNC_PASS){
		return rc;
	}
	mdelay(4);
	write_reg_16bit_value_16bit(o_ctrl,0xDD03, 0x0002); /* I2C SDA strength x1 */
	write_reg_16bit_value_16bit(o_ctrl,0xDD04, 0x0002); /* I2C SDA strength x1 */
	return rc;
}

int dw9785_ois_reset(struct cam_ois_ctrl_t *o_ctrl)
{
	int rc = FUNC_PASS;
	CAM_ERR(CAM_OIS,"[dw9785_ois_reset] start");
	rc = dw9785_device_reset(o_ctrl);
	if(rc != FUNC_PASS){
		return rc;
	}
	dw9785_mcu_enable(o_ctrl);
	CAM_ERR(CAM_OIS,"[dw9785_ois_reset] done : standby");
	return rc;
}

int dw9785_ois_on(struct cam_ois_ctrl_t *o_ctrl)
{
	CAM_ERR(CAM_OIS,"[dw9785_ois_on]");
	write_reg_16bit_value_16bit(o_ctrl,0x7012, 0x0001); /* set control mode */
	mdelay(1);
	write_reg_16bit_value_16bit(o_ctrl,0x7011, 0x0000); /* ois on */
	mdelay(1);
	if(dw9785_wait_check_register(o_ctrl, 0x7010, 0x1000, 3, 10) == FUNC_PASS) { /* busy check */
		CAM_ERR(CAM_OIS,"[dw9785_ois_on] ois on success");
		return FUNC_PASS;
	}

	return FUNC_FAIL;
}

int dw9785_servo_on(struct cam_ois_ctrl_t *o_ctrl)
{
	CAM_ERR(CAM_OIS,"[dw9785_servo_on]");
	write_reg_16bit_value_16bit(o_ctrl,0x7012, 0x0001); /* set control mode */
	mdelay(1);
	write_reg_16bit_value_16bit(o_ctrl,0x7011, 0x0001); /* servo on */
	mdelay(1);
	if(dw9785_wait_check_register(o_ctrl, 0x7010, 0x1001, 3, 10) == FUNC_PASS) { /* busy check */
		CAM_ERR(CAM_OIS,"[dw9785_servo_on] servo on success");
		return FUNC_PASS;
	}

	return FUNC_FAIL;
}

int dw9785_servo_off(struct cam_ois_ctrl_t *o_ctrl)
{
	CAM_ERR(CAM_OIS,"[dw9785_servo_off]");
	write_reg_16bit_value_16bit(o_ctrl,0x7012, 0x0001); /* Set control mode */
	mdelay(1);
	write_reg_16bit_value_16bit(o_ctrl,0x7011, 0x0002); /* servo off */
	mdelay(1);

	if(dw9785_wait_check_register(o_ctrl, 0x7010, 0x1002,  3, 10) == FUNC_PASS) { /* busy check */
		CAM_ERR(CAM_OIS,"[dw9785_servo_off] servo off success");
		return FUNC_PASS;
	}
	return FUNC_FAIL;
}

int dw9785_all_checksum_check(struct cam_ois_ctrl_t *o_ctrl)
{
	/*
	flash_checksum_status : 0xD010
	Bit [0]: fw checksum error
	Bit [1]: data-x checksum status
	Bit [2]: param checksum error
	Bit [3]: module cal. checksum error
	Bit [4]: set cal. checksum error
	Read_verify_checksum : 0xD011
	*/
	uint32_t reg_fw_checksum; /* 0x74FE */
	uint32_t reg_module_cal_checksum; /* 0x76FE */
	uint32_t reg_set_cal_checksum; /* 0x77FE */
	uint32_t reg_flash_checksum_status; /* 0xD010 */
	uint32_t reg_rv_checksum_status; /* 0xD011 */

	read_reg_16bit_value_16bit(o_ctrl,0x74FE, &reg_fw_checksum);
	read_reg_16bit_value_16bit(o_ctrl,0x76FE, &reg_module_cal_checksum);
	read_reg_16bit_value_16bit(o_ctrl,0x77FE, &reg_set_cal_checksum);

	read_reg_16bit_value_16bit(o_ctrl,0xD010, &reg_flash_checksum_status);
	read_reg_16bit_value_16bit(o_ctrl,0xD011, &reg_rv_checksum_status);
	CAM_ERR(CAM_OIS,"[dw9785_all_checksum_check] flash_checksum(0xD010)_status : 0x%04X", reg_flash_checksum_status);
	CAM_ERR(CAM_OIS,"[dw9785_all_checksum_check] read_verify_checksum(0xD011)_status : 0x%04X", reg_rv_checksum_status);
	CAM_ERR(CAM_OIS,"[dw9785_all_checksum_check] FW_checksum(0x74FE) : 0x%04X%04X", reg_fw_checksum >> 8, reg_fw_checksum & 0xff);
	CAM_ERR(CAM_OIS,"[dw9785_all_checksum_check] module_cal_checksum(0x76FE) : 0x%04X%04X", reg_module_cal_checksum >> 8, reg_module_cal_checksum & 0xff);
	CAM_ERR(CAM_OIS,"[dw9785_all_checksum_check] set_cal_checksum(0x77FE) : 0x%04X%04X", reg_set_cal_checksum >> 8, reg_set_cal_checksum & 0xff);		

	if(reg_rv_checksum_status != 0xFF00){
		CAM_ERR(CAM_OIS,"[dw9785_all_checksum_check] reg_rv_checksum error - stop");
		return EOK;
	}

	if(reg_flash_checksum_status == 0x001F)
	{
		CAM_ERR(CAM_OIS,"[dw9785_all_checksum_check] flash_checksum(0xD010) : 0x%04X - pass", reg_flash_checksum_status);
		return EOK;
	}else
	{
		if (!(reg_flash_checksum_status & FLASH_FW_CHECKSUM))
			CAM_ERR(CAM_OIS,"[dw9785_all_checksum_check] fw checksum(bit[0]) : 0x%04X - error", reg_flash_checksum_status);

		if (!(reg_flash_checksum_status & FLASH_DATA_X_CHECKSUM))
			CAM_ERR(CAM_OIS,"[dw9785_all_checksum_check] data_X checksum(bit[1]) : 0x%04X - error", reg_flash_checksum_status);

		if (!(reg_flash_checksum_status & FLASH_PARAM_CHECKSUM))
			CAM_ERR(CAM_OIS,"[dw9785_all_checksum_check] param checksum(bit[2]) : 0x%04X - error", reg_flash_checksum_status);

		if (!(reg_flash_checksum_status & FLASH_MODULE_CAL_CHECKSUM))
			CAM_ERR(CAM_OIS,"[dw9785_all_checksum_check] module cal. checksum(bit[3]) : 0x%04X - error", reg_flash_checksum_status);

		if (!(reg_flash_checksum_status & FLASH_SET_CAL_CHECKSUM))
			CAM_ERR(CAM_OIS,"[dw9785_all_checksum_check] set cal. checksum(bit[4]) : 0x%04X - error", reg_flash_checksum_status);

		return ERROR_FLASH_CHECKSUM;
	}
}

int dw9785_fw_checksum_check(struct cam_ois_ctrl_t *o_ctrl)
{
	/*
	flash_checksum_status : 0xD010
	Bit [0]: fw checksum error
	Bit [1]: data-x checksum status
	Bit [2]: param checksum error
	*/
	uint32_t reg_fw_checksum; /* 0x700C */
	uint32_t reg_flash_checksum_status; /* 0xD010 */
	uint32_t reg_rv_checksum_status; /* 0xD011 */

	//i2c_block_read_reg(0x74FE, reg_fw_checksum, 2);
	read_reg_16bit_value_16bit(o_ctrl,0x74FE, &reg_fw_checksum);
	read_reg_16bit_value_16bit(o_ctrl,0xD010, &reg_flash_checksum_status);
	read_reg_16bit_value_16bit(o_ctrl,0xD011, &reg_rv_checksum_status);

	CAM_ERR(CAM_OIS,"[dw9785_fw_checksum] fw checksum(0x74FE) : 0x%04X%04X", reg_fw_checksum >> 8, reg_fw_checksum & 0xff);

	if(reg_rv_checksum_status == DW9785_AUTORD_RV_OK){
		CAM_ERR(CAM_OIS,"[dw9785_fw_checksum_check] read_verification_checksum : 0x%04X - pass", reg_rv_checksum_status);
	}else{
		CAM_ERR(CAM_OIS,"[dw9785_fw_checksum_check] read_verification_checksum : 0x%04X - fail", reg_rv_checksum_status);
		return ERROR_RV_CHECKSUM;
	}

	if((reg_flash_checksum_status & FLASH_FW_CHECKSUM_OK) == FLASH_FW_CHECKSUM_OK)
	{
		CAM_ERR(CAM_OIS,"[dw9785_fw_checksum_check] flash_checksum(0x%04X) : 0x%04X - pass", 0xD010, reg_flash_checksum_status);
	}else
	{
		if (!((reg_flash_checksum_status & FLASH_FW_CHECKSUM) == FLASH_FW_CHECKSUM))
			CAM_ERR(CAM_OIS,"[dw9785_fw_checksum_check] fw flash checksum(bit[0]) : 0x%04X - error", reg_flash_checksum_status);

		if (!((reg_flash_checksum_status & FLASH_DATA_X_CHECKSUM) == FLASH_DATA_X_CHECKSUM))
			CAM_ERR(CAM_OIS,"[dw9785_fw_checksum_check] data_X flash checksum(bit[1]) : 0x%04X - error", reg_flash_checksum_status);

		if (!((reg_flash_checksum_status & FLASH_PARAM_CHECKSUM) == FLASH_PARAM_CHECKSUM))
			CAM_ERR(CAM_OIS,"[dw9785_fw_checksum_check] param flash checksum(bit[2]) : 0x%04X - error", reg_flash_checksum_status);

		return ERROR_FLASH_CHECKSUM;
	}

	return EOK;
}

struct _FACT_ADJ_DW9785 dw9785_gyro_ofs_calibration(struct cam_ois_ctrl_t *o_ctrl)
{
	/*
	* dw9785 gyro offset calibration
	Error code definition
		-1 : FUNC_FAIL
		0 : No Error
	*/
	int msg = 0;
	uint32_t x_ofs, y_ofs, gyro_status;
	struct _FACT_ADJ_DW9785 FADJCAL = { 0xFFFF, 0xFFFF};
	//unsigned short gyro_ofst_limit;
	CAM_INFO(CAM_OIS,"[dw9785_gyro_ofs_calibration] start");
	write_reg_16bit_value_16bit(o_ctrl,0x7012, 0x0006); /* gyro offset calibration */
	mdelay(10);
	if (dw9785_wait_check_register(o_ctrl, 0x7010, 0x6000, LOOP, DW9785_WAIT_TIME) == FUNC_PASS) {
		write_reg_16bit_value_16bit(o_ctrl,0x7011, 0x0001);
		mdelay(10);
	} else{
		CAM_ERR(CAM_OIS,"[dw9785_gyro_ofs_calibration] gyro offset EXECUTE_ERROR");
		return FADJCAL;
	}
	if(dw9785_wait_check_register(o_ctrl, 0x7010, 0x6001,  LOOP, DW9785_WAIT_TIME) == FUNC_PASS) { /* when calibration is done, Status changes to 0x6001 */
		CAM_INFO(CAM_OIS,"[dw9785_gyro_ofs_calibration]calibration function finish");
	}
	else {
		CAM_ERR(CAM_OIS,"[dw9785_gyro_ofs_calibration]calibration function error");
		return FADJCAL;
	}

	read_reg_16bit_value_16bit(o_ctrl,X_GYRO_OFFSET_ADDR, &x_ofs); /* x gyro offset */
	read_reg_16bit_value_16bit(o_ctrl,Y_GYRO_OFFSET_ADDR, &y_ofs); /* y gyro offset */
	read_reg_16bit_value_16bit(o_ctrl,GYRO_OFS_STATUS, &gyro_status); /* gyro offset status */
	CAM_INFO(CAM_OIS,"[dw9785_gyro_ofs_calibration]x gyro offset: 0x%04X(%d)", x_ofs, (short)x_ofs);
	CAM_INFO(CAM_OIS,"[dw9785_gyro_ofs_calibration]y gyro offset: 0x%04X(%d)", y_ofs, (short)y_ofs);
	CAM_INFO(CAM_OIS,"[dw9785_gyro_ofs_calibration]gyro_status: 0x%04X", gyro_status);

	if( (gyro_status & 0x8000)== 0x8000) {	/* Read Gyro offset cailbration result status */
		if (gyro_status & X_AXIS_GYRO_OFS_PASS) {
			msg = EOK;
			CAM_INFO(CAM_OIS,"[dw9785_gyro_ofs_calibration] x gyro ofs cal pass");
		}else
		{
			msg += X_AXIS_GYRO_OFS_FAIL;
			CAM_ERR(CAM_OIS,"[dw9785_gyro_ofs_calibration] x gyro ofs cal fail");
		}

		if (gyro_status & X_AXIS_GYRO_OFS_OVER_MAX_LIMIT) {
			msg += X_AXIS_GYRO_OFS_OVER_MAX_LIMIT;
			CAM_ERR(CAM_OIS,"[dw9785_gyro_ofs_calibration] x gyro ofs over the max. limit");
		}

		if (gyro_status & Y_AXIS_GYRO_OFS_PASS) {
			msg += EOK;
			CAM_INFO(CAM_OIS,"[dw9785_gyro_ofs_calibration] y gyro ofs cal pass");
		}else
		{
			msg += Y_AXIS_GYRO_OFS_FAIL;
			CAM_ERR(CAM_OIS,"[dw9785_gyro_ofs_calibration] y gyro ofs cal fail");
		}

		if (gyro_status & Y_AXIS_GYRO_OFS_OVER_MAX_LIMIT) {
			msg += Y_AXIS_GYRO_OFS_OVER_MAX_LIMIT;
			CAM_ERR(CAM_OIS,"[dw9785_gyro_ofs_calibration] y gyro ofs over the max. limit");
		}

		if (gyro_status & XY_AXIS_CHECK_GYRO_RAW_DATA) {
			msg += XY_AXIS_CHECK_GYRO_RAW_DATA;
			CAM_INFO(CAM_OIS,"[dw9785_gyro_ofs_calibration] check the x/y gyro raw data");
		}
		CAM_INFO(CAM_OIS,"[dw9785_gyro_ofs_calibration] x/y gyro ofs calibration finish");

		if(msg == EOK)
		{
			msg = dw9785_set_cal_store(o_ctrl);
			write_reg_16bit_value_16bit(o_ctrl,SR_PANTILT_ADDR, COMP_ENABLE); /* Set pantilt bypass for SR Test */
			write_reg_16bit_value_16bit(o_ctrl,POS_COMP_EN, COMP_DISABLE); /* posture compensation off */
		}
		FADJCAL.gl_GX_OFS = x_ofs;
		FADJCAL.gl_GY_OFS = y_ofs;
		CAM_INFO(CAM_OIS,"[dw9785_gyro_ofs_calibration] end pass");
		return FADJCAL;;
	}
	else {
		CAM_ERR(CAM_OIS,"[dw9785_gyro_ofs_calibration] x/y gyro ofs calibration end fail");
		return FADJCAL;;
	}
}

int dw9785_fw_checksum(struct cam_ois_ctrl_t *o_ctrl)
{
	uint32_t reg_fw_checksum; /* 0x74FE */
	uint32_t ref_flash_checksum; /* 0xD010 */
	uint32_t ref_rv_checksum; /* 0xD011 */
	//i2c_block_read_reg(0x74FE, reg_fw_checksum, 2);
	read_reg_16bit_value_16bit(o_ctrl,0x74FE, &reg_fw_checksum);
	read_reg_16bit_value_16bit(o_ctrl,0xD010, &ref_flash_checksum);
	read_reg_16bit_value_16bit(o_ctrl,0xD011, &ref_rv_checksum);
	CAM_ERR(CAM_OIS,"[dw9785_fw_checksum] fw checksum(0x74FE) : 0x%04X%04X", reg_fw_checksum >>8, reg_fw_checksum & 0xff);
	CAM_ERR(CAM_OIS,"[dw9785_fw_checksum] flash checksum(0xD010) : 0x%04X", ref_flash_checksum);
	CAM_ERR(CAM_OIS,"[dw9785_fw_checksum] read-verify checksum(0xD011) : 0x%04X", ref_rv_checksum);

	if(ref_rv_checksum == 0xFF00) CAM_ERR(CAM_OIS,"[dw9785_fw_checksum] fw read-verify checksum : pass");

	if(ref_flash_checksum == 0x001F){
		CAM_ERR(CAM_OIS,"[dw9785_fw_checksum] fw flash checksum : pass");
		return FUNC_PASS;
	}
	else{
		if(0xD010 & 0x0001)	CAM_ERR(CAM_OIS,"[dw9785_fw_checksum] fw checksum fail : checksum(0x%04X)", reg_fw_checksum);
		if(0xD010 & 0x0002)	CAM_ERR(CAM_OIS,"[dw9785_fw_checksum] data_x checksum fail");
		if(0xD010 & 0x0004)	CAM_ERR(CAM_OIS,"[dw9785_fw_checksum] param checksum fail");
		return FUNC_FAIL;
	}
}

int dw9785_cal_mem_checksum(struct cam_ois_ctrl_t *o_ctrl)
{
	uint32_t reg_set_cal_checksum; /* 0x77FE */
	uint32_t ref_flash_checksum; /* 0xD010 */
	uint32_t ref_rv_checksum; /* 0xD011 */

	//i2c_block_read_reg(0x77FE, reg_set_cal_checksum, 2);
	read_reg_16bit_value_16bit(o_ctrl,0x77FE, &reg_set_cal_checksum);
	read_reg_16bit_value_16bit(o_ctrl,0xD010, &ref_flash_checksum);
	read_reg_16bit_value_16bit(o_ctrl,0xD011, &ref_rv_checksum);

	CAM_ERR(CAM_OIS,"[dw9785_fw_checksum] set-cal checksum : 0x%04X%04X", reg_set_cal_checksum >> 8, reg_set_cal_checksum & 0xff);
	CAM_ERR(CAM_OIS,"[dw9785_fw_checksum] flash checksum : 0x%04X", ref_flash_checksum);
	CAM_ERR(CAM_OIS,"[dw9785_fw_checksum] read-verify checksum : 0x%04X", ref_rv_checksum);

	if(ref_rv_checksum == 0xFF00) CAM_ERR(CAM_OIS,"[dw9785_fw_checksum] fw read-verify checksum : pass");

	if(ref_flash_checksum == 0x001F){
		CAM_ERR(CAM_OIS,"[dw9785_fw_checksum] fw flash checksum : pass");
		return FUNC_PASS;
	}
	else{
		if(0xD010 & 0x0010)	CAM_ERR(CAM_OIS,"[dw9785_fw_checksum] set_cal checksum fail");
		return FUNC_FAIL;
	}
}

int dw9785_set_cal_store(struct cam_ois_ctrl_t *o_ctrl)
{
	/*
	Error code definition
	0 : No Error
	-1 : FUNC_FAIL
	*/
	CAM_ERR(CAM_OIS,"[dw9785_set_cal_store] start");
	write_reg_16bit_value_16bit(o_ctrl,0x7012, 0x000A); /* set store mode */
	//When store is done, status changes to 0xA000
	if(dw9785_wait_check_register(o_ctrl, 0x7010, 0xA000,  10, 100) == FUNC_FAIL) {
		CAM_ERR(CAM_OIS,"[dw9785_set_cal_store] failed to enter store mode");
		return FUNC_FAIL;
	}

	CAM_ERR(CAM_OIS,"[dw9785_set_cal_store] successful entry into store mode");
	CAM_ERR(CAM_OIS,"[dw9785_set_cal_store] select set cal mememory(red)");
	write_reg_16bit_value_16bit(o_ctrl,0x700F, 0x5959); /* set protect code */
	mdelay(1);

	CAM_ERR(CAM_OIS,"[dw9785_set_cal_store] execute store");
	write_reg_16bit_value_16bit(o_ctrl,0x7011, 0x0001); /* Execute store */
	mdelay(1);

	/* When calculating checksum is done, status changes to 0x8000 */
	if(dw9785_wait_check_register(o_ctrl, 0x701E, 0x8000, LOOP, DW9785_WAIT_TIME) == FUNC_FAIL) {
		CAM_ERR(CAM_OIS,"[dw9785_set_cal_store] fw calculating checksum fail");
		return FUNC_FAIL;
	}

	CAM_ERR(CAM_OIS,"[dw9785_set_cal_store] fw calculating checksum done");
	CAM_ERR(CAM_OIS,"[dw9785_set_cal_store] red memory protection off");
	write_reg_16bit_value_16bit(o_ctrl,0xDE92, 0xDE56);

	CAM_ERR(CAM_OIS,"[dw9785_set_cal_store] set-cal store start");
	write_reg_16bit_value_16bit(o_ctrl,0xD012, 0x0002);
	mdelay(6);

	if(dw9785_wait_check_register(o_ctrl, 0xDE00, 0x0000, 3, 30) != FUNC_PASS){
		CAM_ERR(CAM_OIS,"[dw9785_set_cal_store] store busy check fail");
		return ERROR_BUSY;
	}

	write_reg_16bit_value_16bit(o_ctrl, 0x701E, 0x8001); /* set checksum done flag for fw*/
	CAM_ERR(CAM_OIS,"[dw9785_set_cal_store] store done");
	if(dw9785_wait_check_register(o_ctrl, 0x7010, 0xA001, LOOP, DW9785_WAIT_TIME) == FUNC_FAIL) {
		CAM_ERR(CAM_OIS,"[dw9785_set_cal_store] store function fail");
		return FUNC_FAIL;
	}
	CAM_ERR(CAM_OIS,"[dw9785_set_cal_store] reboot for auto-read");
	dw9785_ois_reset(o_ctrl);
	if(dw9785_cal_mem_checksum(o_ctrl) == FUNC_PASS)
	    return FUNC_PASS;
	return FUNC_PASS;
}

int dw9785_wait_check_register(struct cam_ois_ctrl_t *o_ctrl, unsigned short reg, unsigned short ref, unsigned short read_cycle, unsigned short wait_delay)
{
	/*
	reg : read target register
	ref : compare reference data
	*/
	//int ret = 0;
	uint32_t r_data;
	for (int i = 0; i < read_cycle; i++) {
		read_reg_16bit_value_16bit(o_ctrl,reg, &r_data);
		if (r_data == ref) {
			CAM_ERR(CAM_OIS,"[wait_check_register] pass (addr : 0x%04X, data : 0x%04X, cnt: %d, delay: %dms)", reg, r_data, i, i * wait_delay);
			return FUNC_PASS;
		}
		mdelay(wait_delay);
	}

	CAM_ERR(CAM_OIS,"[wait_check_register] fail (addr : 0x%04X, data : 0x%04X, cnt: %d, time: %dms)", reg, r_data, read_cycle, read_cycle * wait_delay);

	return FUNC_FAIL;
}

void dw9785_fw_read(struct cam_ois_ctrl_t *o_ctrl)
{
	/* Read the data of fw memory using register */
	//unsigned short DW9785_FW_buffer[DW9785_MCS_SIZE_W/2];
	//int i = 0;
	//CAM_ERR(CAM_OIS,"[dw9785_download_fw] sequential read start");
	//write_reg_16bit_value_16bit(o_ctrl,0xDE04, DW9785_MCS_START_ADDRESS|0x8000);
	//i2c_block_read_reg(0xDE05, DW9785_FW_buffer, DW9785_MCS_SIZE_W/2);
	CAM_ERR(CAM_OIS,"[dw9785_fw_read] finish");
}

void dw9785_ois_initial_setting(struct cam_ois_ctrl_t *o_ctrl)
{
	CAM_ERR(CAM_OIS,"[dw9785_ois_initial_setting] enable tripod mode");
	write_reg_16bit_value_16bit(o_ctrl,TRIPOD_EN_ADDR, COMP_ENABLE); /* enable tripod mode */
	CAM_ERR(CAM_OIS,"[dw9785_ois_initial_setting] enable dd func.");
	write_reg_16bit_value_16bit(o_ctrl,DD_FUNC_EN_ADDR, COMP_ENABLE); /* enable dd func. */
}


void hall_sensitivity_cal_example(struct cam_ois_ctrl_t *o_ctrl)
{
	//unsigned char project_id = 0;
	//uint32_t r_reg = 0;
	//uint32_t gyro_gain_x = 0;
	//uint32_t gyro_gain_y = 0;

	//float hall_sensitivity_x = 0.0;
	//float hall_sensitivity_y = 0.0;

	//read_reg_16bit_value_16bit(o_ctrl,0x7003, &r_reg);		/* msb: set info, lsb: project info */
	//project_id = (unsigned char) (r_reg & 0xFF);

	//read_reg_16bit_value_16bit(o_ctrl,X_GYRO_GAIN_ADDR, &gyro_gain_x);	/* read gyro gain x */
	//read_reg_16bit_value_16bit(o_ctrl,Y_GYRO_GAIN_ADDR, &gyro_gain_y);	/* read gyro gain y */

	//CAM_ERR(CAM_OIS,"[hall_sensitivity_cal] gyro gain x = %d\r\n", gyro_gain_x);
	//CAM_ERR(CAM_OIS,"[hall_sensitivity_cal] gyro gain y = %d\r\n", gyro_gain_y);

	//if( project_id == PJT_CTD2054){
	//	CAM_ERR(CAM_OIS,"[hall_sensitivity_cal] Soli hall sensitivity results:");
	//	hall_sensitivity_x = ( REF_GYRO_RESULT * gyro_gain_x >> 13 ) / (float)REF_STROKE_CADILLAC;		/* hall code/um */
	//	hall_sensitivity_y = ( REF_GYRO_RESULT * gyro_gain_y >> 13 ) / (float)REF_STROKE_CADILLAC;
	//}

	//CAM_ERR(CAM_OIS,"[hall_sensitivity_cal] hall sensitivity x = %.1f\r\n", hall_sensitivity_x);
	//CAM_ERR(CAM_OIS,"[hall_sensitivity_cal] hall sensitivity y = %.1f\r\n", hall_sensitivity_y);
}

void DW9785_WriteGyroGainToFlash(struct cam_ois_ctrl_t *o_ctrl,uint32_t X_gain, uint32_t Y_gain)
{
	uint32_t X_ori, Y_ori;
	read_reg_16bit_value_16bit(o_ctrl, dw9785_Gyro_gain_x, &X_ori);//read 0xB806
	read_reg_16bit_value_16bit(o_ctrl, dw9785_Gyro_gain_y, &Y_ori);//read 0xB808
	CAM_INFO(CAM_OIS, "[DW9785_WriteGyroGainToFlash] oldGyorGain  X_gain= 0x%x  Y_gain= 0x%x",X_ori, Y_ori);
	CAM_INFO(CAM_OIS, "[DW9785_WriteGyroGainToFlash] newGyorGain  X_gain= 0x%x  Y_gain= 0x%x",X_gain, Y_gain);
	write_reg_16bit_value_16bit(o_ctrl,dw9785_Gyro_gain_x, X_gain);
	write_reg_16bit_value_16bit(o_ctrl,dw9785_Gyro_gain_y, Y_gain);
	read_reg_16bit_value_16bit(o_ctrl, dw9785_Gyro_gain_x, &X_ori);//read 0xB806
	read_reg_16bit_value_16bit(o_ctrl, dw9785_Gyro_gain_y, &Y_ori);//read 0xB808
	CAM_INFO(CAM_OIS, "[DW9785_WriteGyroGainToFlash] after write  X_gain= 0x%x  Y_gain= 0x%x",X_ori, Y_ori);
}

void DW9785_StoreGyroGainToFlash(struct cam_ois_ctrl_t *o_ctrl)
{
	uint32_t X_ori, Y_ori;
	read_reg_16bit_value_16bit(o_ctrl, dw9785_Gyro_gain_x, &X_ori);//read 0xB806
	read_reg_16bit_value_16bit(o_ctrl, dw9785_Gyro_gain_y, &Y_ori);//read 0xB808
	CAM_INFO(CAM_OIS, "[DW9785_WriteGyroGainToFlash] store GyorGain  X_gain= 0x%x  Y_gain= 0x%x",X_ori, Y_ori);
	dw9785_set_cal_store(o_ctrl);
}

void dw9785_aging_set_target(struct cam_ois_ctrl_t *o_ctrl)
{
	uint32_t working_period = 0, write_read_interval = 0;
	uint32_t write_pos, write_x7100, write_y7180;
	uint32_t read_x7101, read_y7181;
	uint32_t check_period = 10000;
	uint32_t time_cost = 0;
	uint64_t time_enter = 0, time_middle = 0, time_exit = 0;

	if (o_ctrl->oisaging_set_thread == 0) {
		goto oisaging_thread_exit;
	}

	CAM_INFO(CAM_OIS, "OISAging: test E");
	memset(g_get_actual, 0, sizeof(g_get_actual));
	g_loop_done = 0;

	working_period = o_ctrl->set_target.workingPeriod;
	write_read_interval = o_ctrl->set_target.writeReadInterval;

	for (int i = 0; i < OIS_AGING_LOOP_DATA_NUM; i++)
	{
		time_enter = ktime_get_ns() / 1000;

		write_pos = o_ctrl->set_target.targetCodeMap[i];
		write_x7100 = (write_pos >> 16) & 0xFFFF;
		write_y7180 = write_pos & 0xFFFF;
		write_reg_16bit_value_16bit(o_ctrl, 0x7100, write_x7100);
		write_reg_16bit_value_16bit(o_ctrl, 0x7180, write_y7180);
		CAM_INFO(CAM_OIS, "set target[%03d] pos(x7100, y7180)=(0x%04x, 0x%04x)", i, write_x7100, write_y7180);

		for (int wait = 0; wait < write_read_interval / check_period - 1; wait++) {
			if (o_ctrl->oisaging_set_thread == 0) {
				goto oisaging_thread_exit;
			}
			usleep_range(check_period, check_period + 10);
		}

		time_middle = ktime_get_ns() / 1000;
		time_cost = (uint32_t)(time_middle - time_enter);
		if (time_cost < write_read_interval) {
			if (o_ctrl->oisaging_set_thread == 0) {
				goto oisaging_thread_exit;
			}
			usleep_range(write_read_interval - time_cost, write_read_interval - time_cost + 10);
		}

		read_reg_16bit_value_16bit(o_ctrl, 0x7101, &read_x7101);
		read_reg_16bit_value_16bit(o_ctrl, 0x7181, &read_y7181);
		CAM_INFO(CAM_OIS, "get actual[%03d] pos(x7101, y7181)=(0x%04x, 0x%04x)", i, read_x7101, read_y7181);
		g_get_actual[i] = ((read_x7101 & 0xFFFF) << 16) | (read_y7181 & 0xFFFF);

		time_exit = ktime_get_ns() / 1000;
		time_cost = (uint32_t)(time_exit - time_enter);
		if (time_cost < working_period) {
			if (o_ctrl->oisaging_set_thread == 0) {
				goto oisaging_thread_exit;
			}
			usleep_range(working_period - time_cost, working_period - time_cost + 10);
		}
	}

	CAM_INFO(CAM_OIS, "OISAging: test X");
	g_loop_done = 1;

oisaging_thread_exit:
	CAM_ERR(CAM_OIS, "OISAging: thread need stop");
	return;
}

void dw9785_aging_get_actual(uint32_t get_actual[], uint32_t size)
{
	for (int i = 0; i < size; i++) {
		get_actual[i] = g_get_actual[i];
	}
}

void dw9785_aging_loopdone(uint32_t *loop_done)
{
	*loop_done = g_loop_done;
}

