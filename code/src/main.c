/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */



// #include "initiater.h"

#include "initiater.h"


#define LOG_MODULE_NAME app
#define WORKING_AREA 180
#define THRESHOLD_DEGREES 3



K_SEM_DEFINE(my_sem,0,1);
LOG_MODULE_REGISTER(LOG_MODULE_NAME);

matrix_3x3 azimuth_reading[WORKING_AREA];
int zero_point_index_azimuth;
int16_t min_encoder_search_azimuth = 0;
int16_t max_encoder_search_azimuth = 180;
int increment = 1;




void main(void)
{
	LOG_INF("Hello World! %s\n", CONFIG_BOARD);
	initiate_modules();
	int16_t size = (max_encoder_search_azimuth-min_encoder_search_azimuth)/increment;
	k_sem_take(&my_sem, K_FOREVER);
	sweep_search(0, min_encoder_search_azimuth, max_encoder_search_azimuth,increment);
	get_readings(&azimuth_reading, &size);

	for(int i = 0; i < size; i++){
		printk("Encoder: %d,  delta: %d, zigma: %d \n", azimuth_reading[i].encoder, azimuth_reading[i].delta, azimuth_reading[i].zigma);
	}
	zero_point_index_azimuth = find_zero_point(azimuth_reading, size);
	angle_move_servo(0, zero_point_index_azimuth);

	fine_sweeper(0,10,10,19,azimuth_reading[zero_point_index_azimuth]);

	printk("Azimuth search done\n");
	printk("Main:\n");

	

	// if(err){
	// 	LOG_ERR("err:%d", err);
	// }
	while(1){

	}
}
