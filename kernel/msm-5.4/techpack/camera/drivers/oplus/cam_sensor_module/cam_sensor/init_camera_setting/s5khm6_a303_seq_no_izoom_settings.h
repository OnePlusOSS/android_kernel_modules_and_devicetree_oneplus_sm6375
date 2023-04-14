#define HM6_A303_NO_IZOOM_INIT_SETTINGS_SIZE 31
#define HM6_A303_NO_IZOOM_INIT_SETTINGS_SEGMENT_NUM 3

struct hm6_a303_no_izoom_reg_settings {
	struct cam_sensor_i2c_reg_array reg_setting[HM6_A303_NO_IZOOM_INIT_SETTINGS_SIZE];
	enum camera_sensor_i2c_type random_addr_type;
	enum camera_sensor_i2c_type random_data_type;
	enum camera_sensor_i2c_type seq_addr_type;
	enum camera_sensor_i2c_type seq_data_type;
	unsigned short segment_index[HM6_A303_NO_IZOOM_INIT_SETTINGS_SEGMENT_NUM];
	unsigned short delay[HM6_A303_NO_IZOOM_INIT_SETTINGS_SEGMENT_NUM];
	enum cam_sensor_i2c_cmd_type op_code[HM6_A303_NO_IZOOM_INIT_SETTINGS_SEGMENT_NUM];
	unsigned short segment_size[HM6_A303_NO_IZOOM_INIT_SETTINGS_SEGMENT_NUM];
	unsigned short total_size;
};

struct hm6_a303_no_izoom_reg_settings s5khm6_a303_no_izoom_seq_settings =  {
	.reg_setting =
	{
		{.reg_addr = 0xFCFC, .reg_data = 0x4000, .delay = 0x00, .data_mask = 0x00}, \
		{.reg_addr = 0x0000, .reg_data = 0x01C0, .delay = 0x00, .data_mask = 0x00}, \
		{.reg_addr = 0x0000, .reg_data = 0x1AD6, .delay = 0x00, .data_mask = 0x00}, \
		{.reg_addr = 0xFCFC, .reg_data = 0x4000, .delay = 0x00, .data_mask = 0x00}, \
		{.reg_addr = 0x6010, .reg_data = 0x0001, .delay = 0x7530, .data_mask = 0x00}, \
		{.reg_addr = 0x6218, .reg_data = 0xE9C0, .delay = 0x00, .data_mask = 0x00}, \
		{.reg_addr = 0xF468, .reg_data = 0x0000, .delay = 0x00, .data_mask = 0x00}, \
		{.reg_addr = 0x0136, .reg_data = 0x1800, .delay = 0x00, .data_mask = 0x00}, \
		{.reg_addr = 0xFCFC, .reg_data = 0x2001, .delay = 0x00, .data_mask = 0x00}, \
		{.reg_addr = 0xC8CC, .reg_data = 0x00, .delay = 0x00, .data_mask = 0x00}, \
		{.reg_addr = 0xC8CC, .reg_data = 0x00, .delay = 0x00, .data_mask = 0x00}, \
		{.reg_addr = 0xC8CC, .reg_data = 0x00, .delay = 0x00, .data_mask = 0x00}, \
		{.reg_addr = 0xC8CC, .reg_data = 0x00, .delay = 0x00, .data_mask = 0x00}, \
		{.reg_addr = 0xC8CC, .reg_data = 0x02, .delay = 0x00, .data_mask = 0x00}, \
		{.reg_addr = 0xC8CC, .reg_data = 0x49, .delay = 0x00, .data_mask = 0x00}, \
		{.reg_addr = 0xC8CC, .reg_data = 0x46, .delay = 0x00, .data_mask = 0x00}, \
		{.reg_addr = 0xC8CC, .reg_data = 0xF6, .delay = 0x00, .data_mask = 0x00}, \
		{.reg_addr = 0xC8CC, .reg_data = 0xCF, .delay = 0x00, .data_mask = 0x00}, \
		{.reg_addr = 0xC8CC, .reg_data = 0x60, .delay = 0x00, .data_mask = 0x00}, \
		{.reg_addr = 0xC8CC, .reg_data = 0x09, .delay = 0x00, .data_mask = 0x00}, \
		{.reg_addr = 0xC8CC, .reg_data = 0x68, .delay = 0x00, .data_mask = 0x00}, \
		{.reg_addr = 0xC8CC, .reg_data = 0x48, .delay = 0x00, .data_mask = 0x00}, \
		{.reg_addr = 0xC8CC, .reg_data = 0x83, .delay = 0x00, .data_mask = 0x00}, \
		{.reg_addr = 0xC8CC, .reg_data = 0x70, .delay = 0x00, .data_mask = 0x00}, \
		{.reg_addr = 0xC8CC, .reg_data = 0x47, .delay = 0x00, .data_mask = 0x00}, \
		{.reg_addr = 0xC8CC, .reg_data = 0x20, .delay = 0x00, .data_mask = 0x00}, \
		{.reg_addr = 0xC8CC, .reg_data = 0x00, .delay = 0x00, .data_mask = 0x00}, \
		{.reg_addr = 0xC8CC, .reg_data = 0xD7, .delay = 0x00, .data_mask = 0x00}, \
		{.reg_addr = 0xC8CC, .reg_data = 0x70, .delay = 0x00, .data_mask = 0x00}, \
		{.reg_addr = 0xFCFC, .reg_data = 0x4621, .delay = 0x00, .data_mask = 0x00}, \
		{.reg_addr = 0x1002, .reg_data = 0x0000, .delay = 0x00, .data_mask = 0x00}, \
	},
	.random_addr_type = CAMERA_SENSOR_I2C_TYPE_WORD,
	.random_data_type = CAMERA_SENSOR_I2C_TYPE_WORD,
	.seq_addr_type = CAMERA_SENSOR_I2C_TYPE_WORD,
	.seq_data_type = CAMERA_SENSOR_I2C_TYPE_BYTE,
	.segment_index = {
0, 9, 29
	},
	.delay = {
0, 0, 0
	},
	.op_code = {
0, 2, 0
	},
	.segment_size = {
9, 20, 2
	},
	.total_size = 3,
};
