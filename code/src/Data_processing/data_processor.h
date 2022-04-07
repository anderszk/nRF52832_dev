#include "encoder.h"
#include <zephyr/types.h>



void test_me_2();
int find_local_minima(int data_array[][3]);
void send_data_delta(int16_t rssi, int index);
void send_data_zigma(int16_t rssi, int index);
int16_t get_average(int16_t list[]);
void get_data(int16_t *buffer_data);
void freeData();
