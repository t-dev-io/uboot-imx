#ifndef __DS1337_CLK_H
#define __DS1337_CLK_H

#include <dm.h>

struct ds1337_port_config {
	uint8_t i2c_bus;
	uint8_t addr;
};

struct ds1337_port {
	struct ds1337_port_config cfg;
	struct udevice *i2c_dev;
};

int ds1337_clk_init(void);

#endif /* __DS1337_CLK_H */
