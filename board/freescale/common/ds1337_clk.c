/*
 * Copyright 2019 HMS
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#include <common.h>
#include <i2c.h>
#include <time.h>
#include "ds1337_clk.h"


#define DS1337_REG_CONTROL	0x0e
#define DS1337_BIT_RS2		0x10
#define DS1337_BIT_RS1		0x08

struct ds1337_port *ds1337_p;

struct ds1337_port_config ds1337_cfg = {
	.i2c_bus = 0,
	.addr = 0x68,
};

int ds1337_clk_init(void)
{
    uint8_t val=0;
	int ret=0;
	struct udevice *bus;

	memset(ds1337_p, 0, sizeof(struct ds1337_port));

    ds1337_p->cfg = ds1337_cfg;

	ret = uclass_get_device_by_seq(UCLASS_I2C, ds1337_p->cfg.i2c_bus, &bus);
	if (ret) {
		printf("%s: Can't find bus\n", __func__);
		return -EINVAL;
	}

	ret = dm_i2c_probe(bus, ds1337_p->cfg.addr, 0, &ds1337_p->i2c_dev);
	if (ret) {
		printf("%s: Can't ...find device id=0x%x\n", __func__, ds1337_p->cfg.addr);
		return -ENODEV;
	}

    val = dm_i2c_reg_read(ds1337_p->i2c_dev, DS1337_REG_CONTROL);

    if ((!(val & DS1337_BIT_RS2)) || (!(val & DS1337_BIT_RS1)))
    {
        printf("RTC:   reset to 32.768kHz\n");  
        val |= DS1337_BIT_RS2 | DS1337_BIT_RS1;
        ret = dm_i2c_reg_write(ds1337_p->i2c_dev, DS1337_REG_CONTROL, val);
        if (ret)
        {
            printf("%s reset rtc clk failed , err %d\n", __func__, ret);
            return ret;
        }
    }
    else
    {
        printf("RTC:   32.768kHz\n");  
    }

	return 0;
}
