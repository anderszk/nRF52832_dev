/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <sys/printk.h>
#include <nrfx_qdec.h>

int16_t acc_data = 0;

nrfx_qdec_config_t qdec_def_config = NRFX_QDEC_DEFAULT_CONFIG(3, 4, NULL);

void qdec_envent(nrfx_qdec_event_t event)
{
	acc_data = event.data.report.acc;
}

void main(void)
{
	int err;
	printk("Hello\n");
	err = nrfx_qdec_init(&qdec_def_config, qdec_envent);
	if (err != 0)
	{
		printk("Init failed enc.\n");
		return;
	}

	printk("Running!\n");
}
