#include "observer.h"

/**
 * @brief 
 * 
 * @param state 
 * @param min_encoder_search 
 * @param max_encoder_search 
 * @param increment 
 */
void sweep_search(int state, int16_t min_encoder_search, int16_t max_encoder_search, int increment);
int get_readings(matrix_3x3 *main_readings, int16_t *n);
int16_t fine_sweeper(int state, int threshold_degrees, int threshold_search, int sweep_sector, int zero_point);
void reset_readings();
zeros coarse_search();
zeros fine_search(zeros enc_values);

