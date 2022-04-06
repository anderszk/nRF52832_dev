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

int16_t **azimuth_reading;
int nulls_azimuth;
int min_encoder_search = 0;
int16_t max_encoder_search = 15;
int increment = 1;

void main(void)
{
	LOG_INF("Hello World! %s\n", CONFIG_BOARD);
	// initiate_modules();
	int err = timer_init();
	err = timer_start();
	err = init_bluethooth_scan();	
	azimuth_reading = sweep_search(0, min_encoder_search, max_encoder_search,increment);
	
	printk("Azimuth search done\n");
	// printk("%lu",sizeof(&azimuth_reading));
	for(int i = 0; i < (sizeof(azimuth_reading)/sizeof(azimuth_reading[0])); i++){
		printk("Encoder: %d, delta: %d, zigma: %d \n", azimuth_reading[i][0], azimuth_reading[i][1], azimuth_reading[i][2]);
	}

	// if(err){
	// 	LOG_ERR("err:%d", err);
	// }
	while(1){

	}
}
