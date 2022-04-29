#include <zephyr/types.h>


/**
 * @brief Function for filtering the measured RSSI value.
 * 
 * This function takes in the measured RSSI value and will update the Kalman Gain
 * and the estimated RSSI value based on the static constant values set in the function.
 * It will then update the error covarianse which is used when calculating the estimated RSSI value.
 * The function will return the raw RSSI value for the first RSSI value measured.
 * 
 * 
 * @param U The raw RSSI value sent in the function
 * @return  The estimated RSSI value calculated
 */
int8_t KALMAN_DELTA(int8_t U);

/**
 * @brief Function for filtering the measured RSSI value.
 * 
 * This function takes in the measured RSSI value and will update the Kalman Gain
 * and the estimated RSSI value based on the static constant values set in the function.
 * It will then update the error covarianse which is used when calculating the estimated RSSI value.
 * The function will return the raw RSSI value for the first RSSI value measured.
 * 
 * 
 * @param U The raw RSSI value sent in the function
 * @return  The estimated RSSI value calculated
 */
int8_t KALMAN_ZIGMA(int8_t U);