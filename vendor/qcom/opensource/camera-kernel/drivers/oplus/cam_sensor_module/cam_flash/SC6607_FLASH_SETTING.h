{
    .need_standby_mode = 1,
    .flashprobeinfo =
	{
		.flash_name = "sc6607",
		.slave_write_address = 0x64,
		.flash_id_address = 0x00,
		.flash_id = 0x67,
		.addr_type = CAMERA_SENSOR_I2C_TYPE_BYTE,
		.data_type = CAMERA_SENSOR_I2C_TYPE_BYTE,
	},
    .cci_client =
	{
        .cci_i2c_master = MASTER_0,
        .i2c_freq_mode = I2C_FAST_PLUS_MODE,
        .sid = 0x64 >> 1,
	},
	.flashinitsettings =
	{
		.reg_setting =
		{
			{.reg_addr = 0x80, .reg_data = 0x01, .delay = 0x00, .data_mask = 0x00}, \
			{.reg_addr = 0x81, .reg_data = 0x26, .delay = 0x00, .data_mask = 0x00}, \
			{.reg_addr = 0x82, .reg_data = 0x26, .delay = 0x00, .data_mask = 0x00}, \
			{.reg_addr = 0x83, .reg_data = 0x9f, .delay = 0x00, .data_mask = 0x00}, \
			{.reg_addr = 0x84, .reg_data = 0x02, .delay = 0x00, .data_mask = 0x00}, \
			{.reg_addr = 0x85, .reg_data = 0x02, .delay = 0x00, .data_mask = 0x00}, \
			{.reg_addr = 0x86, .reg_data = 0x02, .delay = 0x00, .data_mask = 0x00}, \
			{.reg_addr = 0x8A, .reg_data = 0x00, .delay = 0x00, .data_mask = 0x00}, \
			{.reg_addr = 0x8B, .reg_data = 0x01, .delay = 0x00, .data_mask = 0x00}, \
		},
		.size = 9,
		.addr_type = CAMERA_SENSOR_I2C_TYPE_BYTE,
		.data_type = CAMERA_SENSOR_I2C_TYPE_BYTE,
		.delay = 1,
	},
	.flashhighsettings =
	{
		.reg_setting =
		{

			{.reg_addr = 0x80, .reg_data = 0xc1, .delay = 0x00, .data_mask = 0x00}, \
		},
		.size = 1,
		.addr_type = CAMERA_SENSOR_I2C_TYPE_BYTE,
		.data_type = CAMERA_SENSOR_I2C_TYPE_BYTE,
		.delay = 1,
	},
	.flashlowsettings =
	{
		.reg_setting =//100ma
		{
			{.reg_addr = 0x80, .reg_data = 0x31, .delay = 0x00, .data_mask = 0x00}, \
		},
		.size = 1,
		.addr_type = CAMERA_SENSOR_I2C_TYPE_BYTE,
		.data_type = CAMERA_SENSOR_I2C_TYPE_BYTE,
		.delay = 1,
	},
	.flashlow1ledsettings =
	{
		.reg_setting =//100ma for TLED1
		{
			{.reg_addr = 0x84, .reg_data = 0x06, .delay = 0x00, .data_mask = 0x00}, \
			{.reg_addr = 0x80, .reg_data = 0x21, .delay = 0x00, .data_mask = 0x00}, \
		},
		.size = 2,
		.addr_type = CAMERA_SENSOR_I2C_TYPE_BYTE,
		.data_type = CAMERA_SENSOR_I2C_TYPE_BYTE,
		.delay = 1,
	},
	.flashlowlevelsettings =
	{

		{
			.reg_setting =//50ma
			{
				{.reg_addr = 0x84, .reg_data = 0x02, .delay = 0x00, .data_mask = 0x00}, \
				{.reg_addr = 0x80, .reg_data = 0x21, .delay = 0x00, .data_mask = 0x00}, \
			},
			.size = 2,
			.addr_type = CAMERA_SENSOR_I2C_TYPE_BYTE,
			.data_type = CAMERA_SENSOR_I2C_TYPE_BYTE,
			.delay = 1,
		},
		{
			.reg_setting =//100ma
			{
				{.reg_addr = 0x84, .reg_data = 0x06, .delay = 0x00, .data_mask = 0x00}, \
				{.reg_addr = 0x80, .reg_data = 0x21, .delay = 0x00, .data_mask = 0x00}, \
			},
			.size = 2,
			.addr_type = CAMERA_SENSOR_I2C_TYPE_BYTE,
			.data_type = CAMERA_SENSOR_I2C_TYPE_BYTE,
			.delay = 1,
		},
		{
			.reg_setting =//125 ma
			{
				{.reg_addr = 0x84, .reg_data = 0x08, .delay = 0x00, .data_mask = 0x00}, \
				{.reg_addr = 0x80, .reg_data = 0x21, .delay = 0x00, .data_mask = 0x00}, \
			},
			.size = 2,
			.addr_type = CAMERA_SENSOR_I2C_TYPE_BYTE,
			.data_type = CAMERA_SENSOR_I2C_TYPE_BYTE,
			.delay = 1,
		},
		{
			.reg_setting =//162.5 ma
			{
				{.reg_addr = 0x84, .reg_data = 0x0B, .delay = 0x00, .data_mask = 0x00}, \
				{.reg_addr = 0x80, .reg_data = 0x21, .delay = 0x00, .data_mask = 0x00}, \
			},
			.size = 2,
			.addr_type = CAMERA_SENSOR_I2C_TYPE_BYTE,
			.data_type = CAMERA_SENSOR_I2C_TYPE_BYTE,
			.delay = 1,
		},
	},
	.flashoffsettings =
	{
		.reg_setting =
		{
			{.reg_addr = 0x80, .reg_data = 0x01, .delay = 0x00, .data_mask = 0x00}, \
		},
		.size = 1,
		.addr_type = CAMERA_SENSOR_I2C_TYPE_BYTE,
		.data_type = CAMERA_SENSOR_I2C_TYPE_BYTE,
		.delay = 1,
	},
	.flashpowerupsettings =
	{
		.single_power =
		{
			{
				.seq_type = SENSOR_VIO,
				.config_val = 0,
				.delay = 5,
			},
		},
		.size = 1,
	},
	.flashpowerdownsettings =
	{
		.single_power =
		{
			{
				.seq_type = SENSOR_VIO,
				.config_val = 0,
				.delay = 1,
			},
		},
		.size = 1,
	},
},
