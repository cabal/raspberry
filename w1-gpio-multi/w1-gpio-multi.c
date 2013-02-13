/*
 * w1-gpio - GPIO w1 bus master driver
 *
 * Copyright (C) 2007 Ville Syrjala <syrjala@sci.fi>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/w1-gpio.h>

#include "/home/cabal/rpi/linux-rpi-3.2.27/drivers/w1/w1.h"
#include "/home/cabal/rpi/linux-rpi-3.2.27/drivers/w1/w1_int.h"

#include <asm/gpio.h>

// here put number of masters
#define MASTERS 2

static struct w1_bus_master *master[MASTERS];

static void w1_gpio_multi_write_bit_dir(void *data, u8 bit)
{
	struct w1_gpio_platform_data *pdata = data;
	if (bit) {
		gpio_direction_input(pdata->pin);
	}
	else {
		gpio_direction_output(pdata->pin, 0);
	}
}

static u8 w1_gpio_multi_read_bit(void *data)
{
	struct w1_gpio_platform_data *pdata = data;
	return gpio_get_value(pdata->pin) ? 1 : 0;
}

static void w1_gpio_multi_cleanup(void)
{
	int i;
	for (i = 0; i < MASTERS; i++) {
		if (master[i] > 0) {
			w1_remove_master_device(master[i]);

			if (master[i]->data > 0) {
				struct w1_gpio_platform_data *pdata = master[i]->data;
				gpio_free(pdata->pin);
				kfree(master[i]->data);
			}

			kfree(master[i]);
		}
	}
}

static int __init w1_gpio_multi_probe(void)
{
	// here put numbers of gpio pins
	int i, err, pins[] = {4, 14};
	struct w1_gpio_platform_data *pdata;

	for (i = 0; i < MASTERS; i++) {
		master[i] = 0;
	}

	for (i = 0; i < MASTERS; i++) {
		master[i] = kzalloc(sizeof(struct w1_bus_master), GFP_KERNEL);
		if (!master[i]) {
			w1_gpio_multi_cleanup();
			return -ENOMEM;
		}

		master[i]->data = kzalloc(sizeof(struct w1_gpio_platform_data), GFP_KERNEL);
		master[i]->read_bit = w1_gpio_multi_read_bit;
		master[i]->write_bit = w1_gpio_multi_write_bit_dir;

		pdata = master[i]->data;
		pdata->pin = pins[i];
		pdata->is_open_drain = 0;

		err = gpio_request(pdata->pin, "w1");
		if (err) {
			w1_gpio_multi_cleanup();
			return err;
		}

		gpio_direction_input(pdata->pin);

		err = w1_add_master_device(master[i]);
		if (err) {
			w1_gpio_multi_cleanup();
			return err;
		}
	}

	return 0;
}

static void __exit w1_gpio_multi_remove(void)
{
	w1_gpio_multi_cleanup();
}

module_init(w1_gpio_multi_probe);
module_exit(w1_gpio_multi_remove);

MODULE_DESCRIPTION("GPIO w1 multi bus master driver");
MODULE_AUTHOR("Ville Syrjala <syrjala@sci.fi>, Cabal <cabal@cabal.pl>");
MODULE_LICENSE("GPL");
