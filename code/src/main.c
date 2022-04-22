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



K_SEM_DEFINE(my_sem,0,1);
LOG_MODULE_REGISTER(LOG_MODULE_NAME);

// matrix_3x3 azimuth_reading[WORKING_AREA_AZIMUTH];
// matrix_3x3 horizontal_readings[WORKING_AREA_HORIZONTAL];
int zero_point_index_azimuth;
int zero_point_index_horizontal;
int16_t min_encoder_search_azimuth = 0;
int16_t max_encoder_search_azimuth = 180;
int16_t min_encoder_search_horizontal = 0;
int16_t max_encoder_search_horizontal = 90;
int increment = 1;
zeros zero_enc_values;





void main(void)
{
	LOG_INF("Hello World! %s\n", CONFIG_BOARD);
	initiate_modules();
	k_sem_take(&my_sem, K_FOREVER);
	zero_enc_values = coarse_search();
	printk("azimuth zero enc: %d, horizontal zero end: %d", zero_enc_values.azimuth_index, zero_enc_values.horizontal_index);
	zero_enc_values = fine_search(zero_enc_values);
	k_msleep(2000000);


	printk("Search is done\n");
	printk("Main:\n");

	


	while(1){
		k_sleep(K_FOREVER);
	}
}
