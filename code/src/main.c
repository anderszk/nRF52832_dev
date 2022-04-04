/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */



#include "initiater.h"
#include "search.h"


#define LOG_MODULE_NAME app

K_SEM_DEFINE(my_sem,0,1);
LOG_MODULE_REGISTER(LOG_MODULE_NAME);

int azimuth_reading[270];
int nulls_azimuth[];
int min_encoder_search = 0;
int max_encoder_search = 270;

void main(void)
{
	LOG_INF("Hello World! %s\n", CONFIG_BOARD);
	initiate_modules();
	test_me();
	test_me_2();
	test_me_3();

}
