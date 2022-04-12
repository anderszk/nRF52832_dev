#include "encoder.h"
#include <zephyr/types.h>
typedef struct data_s
{
    int16_t encoder;
    int16_t delta;
	int16_t zigma;
} matrix_3x3;






void test_me_2();
// int find_local_minima(int data_array[][3]);
void send_data_delta(int16_t rssi, int index);
void send_data_zigma(int16_t rssi, int index);
int16_t get_average(int16_t list[]);
void get_data(matrix_3x3 *buffer_data);
void freeData();
void value_validater(matrix_3x3 *raw_data, int n);
int find_zero_point(matrix_3x3 validated_values[], int n);
bool zero_point_validater(int16_t value_zigma);
void update_matrix(matrix_3x3 *data, int16_t *n);
void set_fake_values(matrix_3x3 *matrix);//brukes for fake verdier.
