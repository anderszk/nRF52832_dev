/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>
#include <sys/printk.h>

#include "gui.h"
#include "photores.h"

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   100

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led1)

#if DT_NODE_HAS_STATUS(LED0_NODE, okay)
#define LED0	DT_GPIO_LABEL(LED0_NODE, gpios)
#define PIN	DT_GPIO_PIN(LED0_NODE, gpios)
#define FLAGS	DT_GPIO_FLAGS(LED0_NODE, gpios)
#else
#define LED0	""
#define PIN	0
#define FLAGS	0
#endif

void main(void)
{
	const struct device *dev;
	int ret;
	int holla;
	int holla1;
	int holla2;
	int holla3;
	int holla4;
	int holla5;
	int btn;

	dev = device_get_binding(LED0);
	if (dev == NULL) {
		return;
	}

	ret = gpio_pin_configure(dev, PIN, GPIO_OUTPUT_ACTIVE | FLAGS);
	if (ret < 0) {
		return;
	}

	holla = gpio_pin_configure(dev, 13, GPIO_INPUT);
	holla1 = gpio_pin_configure(dev, 12, GPIO_OUTPUT_ACTIVE | FLAGS);
	holla2 = gpio_pin_configure(dev, 11, GPIO_OUTPUT_ACTIVE | FLAGS);
	holla3 = gpio_pin_configure(dev, 2, GPIO_OUTPUT_ACTIVE | FLAGS);
	holla4 = gpio_pin_configure(dev, 3, GPIO_OUTPUT_ACTIVE | FLAGS);
	holla5 = gpio_pin_configure(dev, 4, GPIO_OUTPUT_ACTIVE | FLAGS);
	btn = gpio_pin_configure(dev, 24, GPIO_OUTPUT_ACTIVE | FLAGS);

	while (1) {
		if(gpio_pin_get_raw(dev, 13) == 0){
			gpio_pin_set(dev, 24, 0);
			printk("holla\n");
			gpio_pin_set(dev, 12, 0);
			k_msleep(300);
			gpio_pin_set(dev, 11, 0);
			k_msleep(300);
			gpio_pin_set(dev, 2, 0);
			k_msleep(300);
			gpio_pin_set(dev, 3, 0);
			k_msleep(300);
			gpio_pin_set(dev, 4, 0);
			k_msleep(300);
		}
		else{
			gpio_pin_set(dev, 24, 1);
			printk("alloh\n");
			gpio_pin_set(dev, 4, 0);
			k_msleep(300);
			gpio_pin_set(dev, 3, 0);
			k_msleep(300);
			gpio_pin_set(dev, 2, 0);
			k_msleep(300);
			gpio_pin_set(dev, 11, 0);
			k_msleep(300);
			gpio_pin_set(dev, 12, 0);
			k_msleep(300);
		}
		k_msleep(300);
	}
}
