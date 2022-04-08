#include "observer.h"
typedef struct data_s
{
    int16_t x;
    int16_t y;
	int16_t z;
} matrix_3x3;


void test_me();
void sweep_search(bool state, int16_t min_encoder_search, int16_t max_encoder_search, int increment);
int getReadings(matrix_3x3 *main_readings, int n);