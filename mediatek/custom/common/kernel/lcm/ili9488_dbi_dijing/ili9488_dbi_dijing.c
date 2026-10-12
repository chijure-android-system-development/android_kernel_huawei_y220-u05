#include <linux/string.h>

#include "lcm_drv.h"

#define FRAME_WIDTH  320
#define FRAME_HEIGHT 480

static LCM_UTIL_FUNCS lcm_util;

#define SET_RESET_PIN(v) (lcm_util.set_reset_pin((v)))
#define MDELAY(n)         (lcm_util.mdelay((n)))

static void send_ctrl_cmd(unsigned int cmd)
{
	lcm_util.send_cmd(cmd);
}

static void send_data_cmd(unsigned int data)
{
	lcm_util.send_data(data);
}

static unsigned int read_data_cmd(void)
{
	return lcm_util.read_data();
}

/* Recovered from the stock Y220-U05 C482B206SP05 kernel. */
static void lcm_init(void)
{
	SET_RESET_PIN(1); MDELAY(25);
	SET_RESET_PIN(0); MDELAY(25);
	SET_RESET_PIN(1); MDELAY(120);

	send_ctrl_cmd(0xE0);
	send_data_cmd(0x00); send_data_cmd(0x03); send_data_cmd(0x12);
	send_data_cmd(0x09); send_data_cmd(0x18); send_data_cmd(0x0A);
	send_data_cmd(0x42); send_data_cmd(0x87); send_data_cmd(0x4D);
	send_data_cmd(0x0B); send_data_cmd(0x0F); send_data_cmd(0x0A);
	send_data_cmd(0x18); send_data_cmd(0x1B); send_data_cmd(0x0F);

	send_ctrl_cmd(0xE1);
	send_data_cmd(0x00); send_data_cmd(0x15); send_data_cmd(0x1B);
	send_data_cmd(0x03); send_data_cmd(0x0F); send_data_cmd(0x05);
	send_data_cmd(0x32); send_data_cmd(0x24); send_data_cmd(0x45);
	send_data_cmd(0x02); send_data_cmd(0x0A); send_data_cmd(0x09);
	send_data_cmd(0x33); send_data_cmd(0x39); send_data_cmd(0x0F);

	send_ctrl_cmd(0xC0); send_data_cmd(0x17); send_data_cmd(0x15);
	send_ctrl_cmd(0xC1); send_data_cmd(0x41);
	send_ctrl_cmd(0xC5); send_data_cmd(0x00); send_data_cmd(0x09);
	send_data_cmd(0x80);
	send_ctrl_cmd(0x36); send_data_cmd(0x98);
	send_ctrl_cmd(0x3A); send_data_cmd(0x66);
	send_ctrl_cmd(0xB1); send_data_cmd(0xC0);
	send_ctrl_cmd(0xB4); send_data_cmd(0x02);
	send_ctrl_cmd(0xB6); send_data_cmd(0x02); send_data_cmd(0x62);
	send_ctrl_cmd(0xF7); send_data_cmd(0xA9); send_data_cmd(0x51);
	send_data_cmd(0x2C); send_data_cmd(0x82);
	send_ctrl_cmd(0x35); send_data_cmd(0x00);
	send_ctrl_cmd(0x44); send_data_cmd(0x01); send_data_cmd(0xE0);
	send_ctrl_cmd(0x11); MDELAY(120);
	send_ctrl_cmd(0x29);
}

static void lcm_suspend(void)
{
	send_ctrl_cmd(0x28); MDELAY(10);
	send_ctrl_cmd(0x10); MDELAY(120);
}

static void lcm_resume(void)
{
	send_ctrl_cmd(0x11); MDELAY(120);
	send_ctrl_cmd(0x29); MDELAY(120);
}

static void lcm_update(unsigned int x, unsigned int y,
		       unsigned int width, unsigned int height)
{
	unsigned int x1 = x + width - 1;
	unsigned int y1 = y + height - 1;

	send_ctrl_cmd(0x2A);
	send_data_cmd((x >> 8) & 0xff); send_data_cmd(x & 0xff);
	send_data_cmd((x1 >> 8) & 0xff); send_data_cmd(x1 & 0xff);
	send_ctrl_cmd(0x2B);
	send_data_cmd((y >> 8) & 0xff); send_data_cmd(y & 0xff);
	send_data_cmd((y1 >> 8) & 0xff); send_data_cmd(y1 & 0xff);
	send_ctrl_cmd(0x2C);
}

static unsigned int lcm_esd_check(void)
{
	unsigned int value;

	send_ctrl_cmd(0x0A);
	read_data_cmd();
	value = read_data_cmd() & 0xff;
	return value != 0x9c;
}

static unsigned int lcm_esd_recover(void)
{
	lcm_init();
	MDELAY(10);
	return 1;
}

static void lcm_get_params(LCM_PARAMS *params)
{
	memset(params, 0, sizeof(*params));
	params->type = LCM_TYPE_DBI;
	params->ctrl = LCM_CTRL_PARALLEL_DBI;
	params->width = FRAME_WIDTH;
	params->height = FRAME_HEIGHT;
	params->io_select_mode = 3;

	params->dbi.port = 0;
	params->dbi.clock_freq = LCM_DBI_CLOCK_FREQ_52M;
	params->dbi.data_width = LCM_DBI_DATA_WIDTH_18BITS;
	params->dbi.data_format.color_order = LCM_COLOR_ORDER_RGB;
	params->dbi.data_format.trans_seq = LCM_DBI_TRANS_SEQ_MSB_FIRST;
	params->dbi.data_format.padding = LCM_DBI_PADDING_ON_LSB;
	params->dbi.data_format.format = LCM_DBI_FORMAT_RGB666;
	params->dbi.data_format.width = LCM_DBI_DATA_WIDTH_18BITS;
	params->dbi.cpu_write_bits = LCM_DBI_CPU_WRITE_32_BITS;
	params->dbi.io_driving_current = LCM_DRIVING_CURRENT_8MA;
	params->dbi.te_mode = LCM_DBI_TE_MODE_VSYNC_ONLY;

	params->dbi.parallel.write_setup = 2;
	params->dbi.parallel.write_hold = 2;
	params->dbi.parallel.write_wait = 5;
	params->dbi.parallel.read_latency = 31;
	params->dbi.parallel.wait_period = 8;
}

static void lcm_set_util_funcs(const LCM_UTIL_FUNCS *util)
{
	memcpy(&lcm_util, util, sizeof(lcm_util));
}

LCM_DRIVER ili9488_dbi_dijing_lcm_drv = {
	.name = "ili9488_dbi_dijing",
	.set_util_funcs = lcm_set_util_funcs,
	.get_params = lcm_get_params,
	.init = lcm_init,
	.suspend = lcm_suspend,
	.resume = lcm_resume,
	.update = lcm_update,
	.esd_check = lcm_esd_check,
	.esd_recover = lcm_esd_recover,
};
