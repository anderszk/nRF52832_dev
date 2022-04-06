#include "encoder.h"
#include <zephyr/types.h>



void test_me_2();
int find_local_minima(int data_array[][3]);
void send_data_delta(int8_t rssi, int index);
void send_data_zigma(int8_t rssi, int index);
int8_t get_average(int8_t list[]);
int16_t *get_data();
