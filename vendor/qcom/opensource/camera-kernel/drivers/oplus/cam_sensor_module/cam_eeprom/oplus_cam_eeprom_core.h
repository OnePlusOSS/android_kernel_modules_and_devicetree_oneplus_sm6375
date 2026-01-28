/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2017-2018, The Linux Foundation. All rights reserved.
 */
#ifndef _OPLUS_CAM_EEPROM_CORE_H_
#define _OPLUS_CAM_EEPROM_CORE_H_

#include "cam_eeprom_dev.h"
	int oplus_cam_eeprom_read_memory(struct cam_eeprom_ctrl_t *e_ctrl, struct cam_eeprom_memory_map_t *emap, int j, uint8_t *memptr);
	int writeWord(struct cam_eeprom_ctrl_t *e_ctrl, uint32_t addr, uint32_t data);
	int readDword(struct cam_eeprom_ctrl_t *e_ctrl,uint32_t addr, uint32_t* data);
#endif
/* _OPLUS_CAM_EEPROM_CORE_H_ */
