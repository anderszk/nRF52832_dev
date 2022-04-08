/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */



// #include "initiater.h"
#include "search.h"


#define LOG_MODULE_NAME app



K_SEM_DEFINE(my_sem,0,1);
LOG_MODULE_REGISTER(LOG_MODULE_NAME);

matrix_3x3 azimuth_reading[];
int nulls_azimuth;
int16_t min_encoder_search = 0;
int16_t max_encoder_search = 270;
int increment = 1;

void main(void)
{
	LOG_INF("Hello World! %s\n", CONFIG_BOARD);
	int err = timer_init();
	err = timer_start();
	err = init_bluethooth_scan();	
	int16_t size = (max_encoder_search-min_encoder_search)/increment;
	sweep_search(0, min_encoder_search, max_encoder_search,increment);
	getReadings(&azimuth_reading, size);
	// printk("Azimuth search done\n");
	// // printk("%lu",sizeof(&azimuth_reading));
	printk("Main:\n");
	for(int i = 0; i < size; i++){
		printk("Encoder: %d, delta: %d, zigma: %d \n", azimuth_reading[i].x, azimuth_reading[i].y, azimuth_reading[i].z);
	}
	printk("hhhhhh");

	// if(err){
	// 	LOG_ERR("err:%d", err);
	// }
	while(1){

	}
}
