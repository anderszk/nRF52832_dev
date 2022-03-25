/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <logging/log.h>
#include <dk_buttons_and_leds.h>
#include <sys/printk.h>
#include "observer.h"
#include "button_leds.h"




void main(void)
{
	
	configure_dk_buttons_leds();
	init_bluethooth_scan();
}
