/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */



// #include "initiater.h"

#include "initiater.h"


#define LOG_MODULE_NAME app
#define WORKING_AREA_AZIMUTH 180
#define WORKING_AREA_HORIZONTAL 90
#define THRESHOLD_DEGREES 3



K_SEM_DEFINE(my_sem,0,1);
LOG_MODULE_REGISTER(LOG_MODULE_NAME);

matrix_3x3 azimuth_reading[WORKING_AREA_AZIMUTH];
matrix_3x3 horizontal_readings[WORKING_AREA_HORIZONTAL];
int zero_point_index_azimuth;
int zero_point_index_horizontal;
int16_t min_encoder_search_azimuth = 0;
int16_t max_encoder_search_azimuth = 180;
int16_t min_encoder_search_horizontal = 0;
int16_t max_encoder_search_horizontal = 90;
int increment = 1;





void main(void)
{
	LOG_INF("Hello World! %s\n", CONFIG_BOARD);
	initiate_modules();
	int16_t size = (max_encoder_search_azimuth-min_encoder_search_azimuth)/increment;
	k_sem_take(&my_sem, K_FOREVER);

	set_average_counter(1);
	sweep_search(0, min_encoder_search_azimuth, max_encoder_search_azimuth,increment);
	get_readings(&azimuth_reading, &size);
	for(int i = 0; i < size; i++){
		printk("Encoder: %d,  delta: %d, zigma: %d \n", azimuth_reading[i].encoder, azimuth_reading[i].delta, azimuth_reading[i].zigma);
	}
	zero_point_index_azimuth = find_zero_point(azimuth_reading, size);
	angle_slow_move(0, azimuth_reading[zero_point_index_azimuth].encoder);
	printk("zero value azimuth: %d",azimuth_reading[zero_point_index_azimuth].encoder);
	k_msleep(1000);
	angle_move_servo(2, 90);
	k_msleep(2000);
	set_average_counter(3);

	size = (max_encoder_search_horizontal-min_encoder_search_horizontal)/increment;
	sweep_search(1,min_encoder_search_horizontal, max_encoder_search_horizontal, increment);
	get_readings(&horizontal_readings, &size);
	for(int i = 0; i < size; i++){
		printk("Encoder: %d,  delta: %d, zigma: %d \n", horizontal_readings[i].encoder, horizontal_readings[i].delta, horizontal_readings[i].zigma);
	}
	zero_point_index_horizontal = find_zero_point(horizontal_readings, size);
	angle_slow_move(1, horizontal_readings[zero_point_index_horizontal].encoder);
	k_msleep(1000);
	angle_move_servo(2,0);
	k_msleep(1000);
	fine_sweeper(0,10,10,19,azimuth_reading[zero_point_index_azimuth]);
	k_msleep(1000);
	angle_move_servo(2,90);
	k_msleep(1000);
	fine_sweeper(1, 10, 10, 15, horizontal_readings[zero_point_index_horizontal]);
	k_msleep(1000);
	angle_move_servo(2,0);



	


	printk("Azimuth search done\n");
	printk("Main:\n");

	


	while(1){

	}
}
