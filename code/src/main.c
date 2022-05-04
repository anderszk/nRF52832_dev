/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */


#include "initiater.h"
#include "nrfx_qdec.h"
#include <stdio.h>

K_SEM_DEFINE(my_sem,0,1);

zeros zero_enc_values;


void main(void)
{
	printk("Hello Nordic Semiconductor!\n");
	int err = initiate_modules();
	if(err){
		printk("Error while initiating modules (error: %d). \n", err);
		exit();
	}
	k_sem_take(&my_sem, K_FOREVER);
	k_msleep(2000);
	
	zero_enc_values = coarse_search();
	printk("azimuth zero enc: %d, horizontal zero end: %d\n", zero_enc_values.azimuth, zero_enc_values.elevation);
	zero_enc_values = fine_search(zero_enc_values);
	printk("Search is done\n");
	printk("Zeroes found at Azimuth: %d, Elevation: %d\n", zero_enc_values.azimuth, zero_enc_values.elevation);
	angle_move_servo(0,zero_enc_values.azimuth);
	angle_move_servo(1, zero_enc_values.elevation);
	laser_set(0);

	while(1){
		k_sleep(K_FOREVER);
	}
}
