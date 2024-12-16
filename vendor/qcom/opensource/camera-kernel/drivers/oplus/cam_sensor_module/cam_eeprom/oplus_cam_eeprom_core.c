
#include <linux/module.h>
#include <linux/crc32.h>
#include <media/cam_sensor.h>

#include "cam_eeprom_core.h"
#include "cam_eeprom_soc.h"
#include "cam_debug_util.h"
#include "cam_common_util.h"
#include "cam_packet_util.h"
#include "oplus_cam_eeprom_core.h"

#define         MAX_READ_SIZE           0x7FFFF

int writeWord(struct cam_eeprom_ctrl_t *e_ctrl, uint32_t addr, uint32_t data)
{
	int32_t rc = 0;
	int retry = 3;
	int i = 0;
	struct cam_sensor_i2c_reg_array i2c_write_setting = {
		.reg_addr = addr,
		.reg_data = data,
		.delay = 0x00,
		.data_mask = 0x00,
	};
	struct cam_sensor_i2c_reg_setting i2c_write = {
		.reg_setting = &i2c_write_setting,
		.size = 1,
		.addr_type = CAMERA_SENSOR_I2C_TYPE_WORD,
		.data_type = CAMERA_SENSOR_I2C_TYPE_WORD,
		.delay = 0x00,
	};
	if (e_ctrl == NULL)
	{
		CAM_ERR(CAM_EEPROM, "Invalid Args");
		return -EINVAL;
	}

	for(i = 0; i < retry; i++)
	{
		rc = camera_io_dev_write(&(e_ctrl->io_master_info), &i2c_write);
		if (rc < 0) {
			CAM_ERR(CAM_EEPROM, "write 0x%04x=0x%x failed, retry:%d", addr, data, i+1);
		} else {
			CAM_DBG(CAM_EEPROM, "write 0x%04x = 0x%04x", addr, data);
			return rc;
		}
	}
	return rc;
}

int readDword(struct cam_eeprom_ctrl_t *e_ctrl, uint32_t addr, uint32_t* data) {
	int32_t rc = 0;
	int retry = 3;
	int i;

	if (e_ctrl == NULL)
	{
		CAM_ERR(CAM_EEPROM, "Invalid Args");
		return -EINVAL;
	}

	for(i = 0; i < retry; i++)
	{
		rc = camera_io_dev_read(&(e_ctrl->io_master_info), (uint32_t)addr, (uint32_t*)data,
		                        CAMERA_SENSOR_I2C_TYPE_WORD, CAMERA_SENSOR_I2C_TYPE_DWORD);

		if (rc < 0)
		{
			CAM_ERR(CAM_EEPROM, "read 0x%04x failed, retry:%d", addr, i+1);
		}
		else
		{
			CAM_DBG(CAM_EEPROM, "read 0x%04x = 0x%x", addr, *data);
			return rc;
		}
	}
	return rc;
}

int oplus_cam_eeprom_read_memory(struct cam_eeprom_ctrl_t *e_ctrl, struct cam_eeprom_memory_map_t *emap, int j, uint8_t *memptr) {
	int                                rc = 0;
	uint32_t                           data;
	int                                i = 0, size = 0;
	uint32_t                           address = 0x0000;

	writeWord(e_ctrl,0xD002, 0x0001);
	mdelay(4);
	writeWord(e_ctrl,0xDD03, 0x0002);
	writeWord(e_ctrl,0xDD04, 0x0002);

	size = (emap[j].mem.valid_size + 1) / 4;
	rc = writeWord(e_ctrl, 0xDE01, 0x0040);
	for(i = 0; i < size; i++ ) {
		address = i * 4;
		rc = writeWord(e_ctrl, 0xDE04, 0x8000 | address);
		rc = readDword(e_ctrl, 0xDE05, &data);
		memptr[i*4] = ((data >> 24) & 0xff);
		memptr[i*4+1] = ((data >> 16) & 0xff);
		memptr[i*4+2] = ((data >> 8) & 0xff);
		memptr[i*4+3] = (data & 0xff);
	}
	rc = writeWord(e_ctrl, 0xDE01, 0x0000);
	return rc;
}
