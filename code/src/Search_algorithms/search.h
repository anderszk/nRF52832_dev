#include "observer.h"


void test_me();
void sweep_search(bool state, int16_t min_encoder_search, int16_t max_encoder_search, int increment);
int get_readings(matrix_3x3 *main_readings, int n);
void fine_sweeper(bool state, int threshold_degrees, int threshold_search, int sweep_sector, matrix_3x3 zero_point);

