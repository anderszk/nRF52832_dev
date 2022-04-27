#include "encoder.h"
#include <zephyr/types.h>

typedef struct data_s
{
    int16_t encoder;
    int16_t delta;
	int16_t zigma;
} matrix_x3;

typedef struct data_a
{
    int16_t azimuth;
    int16_t elevation;
} zeros;






void send_data_delta(int16_t rssi, int index);
void send_data_zigma(int16_t rssi, int index);
void set_average_counter(int16_t value);
int16_t get_average(int16_t list[]);
void get_data(matrix_x3 *buffer_data, int N);
void value_validater(matrix_x3 *raw_data, int16_t *n);
void update_matrix(matrix_x3 *data, int16_t *n);
bool zero_point_validater(int16_t value_zigma, int16_t value_delta);
int find_zero_point(matrix_x3 validated_values[], int n);

