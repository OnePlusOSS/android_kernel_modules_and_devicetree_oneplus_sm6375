/**
  ******************************************************************************
  * File Name          : DW9785_SET_API.H
  * Description        : Main program c file
  * DongWoon Anatech   :
  * Version            : 0.1
  ******************************************************************************
  *
  * COPYRIGHT(c) 2023 DWANATECH
  * DW9785 Setup Program for SET
  * Revision History
  * 2023.10.25 by JH Seo -- Draft
  ******************************************************************************
**/
struct _FACT_ADJ_DW9785{
	uint32_t	gl_GX_OFS;
	uint32_t	gl_GY_OFS;
};

//int debug_printf(char *buf, ...);
//void os_mdelay(int delay);
int dw9785_download_open_camera(struct cam_ois_ctrl_t *o_ctrl);
unsigned short dw9785_product_id_check(struct cam_ois_ctrl_t *o_ctrl);
unsigned short dw9785_chip_id_check(struct cam_ois_ctrl_t *o_ctrl);
unsigned short dw9785_fw_ver_check(struct cam_ois_ctrl_t *o_ctrl);
int dw9785_fw_type(struct cam_ois_ctrl_t *o_ctrl);
void dw9785_fw_info(struct cam_ois_ctrl_t *o_ctrl);
int dw9785_download_fw(struct cam_ois_ctrl_t *o_ctrl);
int dw9785_download_fw_reply(struct cam_ois_ctrl_t *o_ctrl);
void dw9785_prog_pt_off(struct cam_ois_ctrl_t *o_ctrl);
int dw9785_fw_eflash_erase(struct cam_ois_ctrl_t *o_ctrl);
void dw9785_mcu_disable(struct cam_ois_ctrl_t *o_ctrl);
int dw9785_mcu_enable(struct cam_ois_ctrl_t *o_ctrl);
void dw9785_chip_enable(struct cam_ois_ctrl_t *o_ctrl);
void dw9785_wakeup(struct cam_ois_ctrl_t *o_ctrl);
void dw9785_deep_sleep(struct cam_ois_ctrl_t *o_ctrl);
int dw9785_device_reset(struct cam_ois_ctrl_t *o_ctrl);
int dw9785_ois_reset(struct cam_ois_ctrl_t *o_ctrl);
int dw9785_ois_on(struct cam_ois_ctrl_t *o_ctrl);
int dw9785_servo_on(struct cam_ois_ctrl_t *o_ctrl);
int dw9785_servo_off(struct cam_ois_ctrl_t *o_ctrl);
int dw9785_all_checksum_check(struct cam_ois_ctrl_t *o_ctrl);
int dw9785_fw_checksum_check(struct cam_ois_ctrl_t *o_ctrl);
struct _FACT_ADJ_DW9785 dw9785_gyro_ofs_calibration(struct cam_ois_ctrl_t *o_ctrl);
int dw9785_set_cal_store(struct cam_ois_ctrl_t *o_ctrl);
int dw9785_wait_check_register(struct cam_ois_ctrl_t *o_ctrl, unsigned short reg, unsigned short ref, unsigned short read_cycle, unsigned short wait_delay);
void dw9785_fw_read(struct cam_ois_ctrl_t *o_ctrl);
void dw9785_ois_initial_setting(struct cam_ois_ctrl_t *o_ctrl);
void hall_sensitivity_cal_example(struct cam_ois_ctrl_t *o_ctrl);
int dw9785_ready_check(struct cam_ois_ctrl_t *o_ctrl);
void DW9785_WriteGyroGainToFlash(struct cam_ois_ctrl_t *o_ctrl,uint32_t X_gain, uint32_t Y_gain);
void DW9785_StoreGyroGainToFlash(struct cam_ois_ctrl_t *o_ctrl);
void dw9785_aging_set_target(struct cam_ois_ctrl_t *o_ctrl);
void dw9785_aging_get_actual(uint32_t get_actual[], uint32_t size);
void dw9785_aging_loopdone(uint32_t *loop_done);

#define LOOP					30
#define DW9785_WAIT_TIME				100

#define DW9785_CHIP_ID			0x9785

/* fw downlaod register*/
#define TRIPOD_EN_ADDR			0x7019
#define DD_FUNC_EN_ADDR			0x701A
#define SR_PANTILT_ADDR			0x701C
#define PROG_MEM_SEL			0x0020

/* set cal register*/
#define X_GYRO_OFFSET_ADDR		0x7700
#define Y_GYRO_OFFSET_ADDR		0x7701
#define X_GYRO_GAIN_ADDR		0x7702
#define Y_GYRO_GAIN_ADDR		0x7703
#define X_GYRO_POL_ADDR			0x7704
#define Y_GYRO_POL_ADDR			0x7705
#define X_REG_MAT_COS_ADDR		0x7706
#define Y_REG_MAT_COS_ADDR		0x7708
#define X_REG_MAT_SIN_ADDR		0x7707
#define Y_REG_MAT_SIN_ADDR		0x7709
#define GYRO_SELECT_ADDR		0x7710
#define GYRO_OFS_STATUS			0x7711
#define GYRO_TOP_BOTTOM			0x7717

#define COILFLUX_COMP_EN		0x7680
#define CROSS_COMP_EN			0x7681
#define LIN_COMP_EN				0x7682
#define AF_COMP_EN				0x7683
#define POS_COMP_EN				0x7684

/* value */                             
#define COMP_ENABLE				0x8000
#define COMP_DISABLE			0x0000

#define MODULE_FW				0
#define SET_FW					1

#define EIS_VSYNC				0
#define EIS_QTIME				1

#define FUNC_PASS				0
#define FUNC_FAIL				-1

/* flash checksum bit information */
#define FLASH_FW_CHECKSUM					0x0001
#define FLASH_DATA_X_CHECKSUM				0x0002
#define FLASH_PARAM_CHECKSUM				0x0004
#define FLASH_MODULE_CAL_CHECKSUM			0x0008
#define FLASH_SET_CAL_CHECKSUM				0x0010

/* fw download error code */
#define EOK 							0
#define ERROR_PRODUCTID					1 /* 0xD016 */
#define ERROR_CHIPID					2 /* 0x7000 */
#define ERROR_FW_ERASE					3
#define ERROR_FW_VALID					4
#define ERROR_FW_VERIFY					5
#define ERROR_FW_CHECKSUM				6
#define ERROR_FW_DOWN_FMC				7
#define ERROR_FW_DOWN_FAIL				8
#define ERROR_RV_CHECKSUM				9
#define ERROR_FLASH_CHECKSUM			10
#define ERROR_WHO_AMI					11
#define ERROR_BUSY						12

/* checksum */
#define FLASH_CHECKSUM_OK				0x001F
#define FLASH_FW_CHECKSUM_OK			(FLASH_FW_CHECKSUM|FLASH_DATA_X_CHECKSUM|FLASH_PARAM_CHECKSUM)
#define DW9785_AUTORD_RV_OK				0xFF00

#define DW9785_MCS_START_ADDRESS				0x0000
#define DW9785_MCS_SIZE_W						32256	/* 31.5KB */
#define PKT_SIZE 						16

/* gyro offset calibration */
#define GYRO_OFS_CAL_DONE_FAIL			0xFF
#define DW9785_EXECUTE_ERROR					-2
#define X_AXIS_GYRO_OFS_PASS			0x1
#define X_AXIS_GYRO_OFS_FAIL			0x1
#define Y_AXIS_GYRO_OFS_PASS			0x2
#define Y_AXIS_GYRO_OFS_FAIL			0x2
#define X_AXIS_GYRO_OFS_OVER_MAX_LIMIT	0x10
#define Y_AXIS_GYRO_OFS_OVER_MAX_LIMIT	0x20
#define XY_AXIS_CHECK_GYRO_RAW_DATA		0x800

#define GYRO_FRONT_LAYOUT		0
#define GYRO_BACK_LAYOUT		1

#define GYRO_DEGREE_0			0
#define GYRO_DEGREE_90			90
#define GYRO_DEGREE_180			180
#define	GYRO_DEGREE_270			270

/* gyro type */
#define ST_LSM6DSM				0x02
#define ST_LSM6DSOQ	  			0x04
#define INVEN_ICM42631			0x05
#define BOSCH_BMI260			0x06
#define INVEN_ICM42602			0x07

/* project info */
#define PJT_CTD2054				0xFF

/* actuator id */
#define ACT_TDK					0x00
#define ACT_MTM					0x01
#define ACT_JAHWA				0x02
#define ACT_SEMCO				0x03
#define ACT_ZET					0x04
#define ACT_SHICOH				0x05
#define ACT_BILLU				0x07



/* ref_stroke[um/deg] */
#define REF_STROKE_CADILLAC 84		/* efl = 4.81mm */

/* ref_gyro_result [code/deg] */
#define REF_GYRO_RESULT			1000

#define dw9785_Gyro_gain_x                      0x7702
#define dw9785_Gyro_gain_y                      0x7703


