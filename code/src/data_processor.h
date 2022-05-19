#include "encoder.h"
#include <zephyr/types.h>

/**
 * @brief Minimum value for RSSI
 * Minimum valid value for RSSI measurement, if values goes under this it becomes
 * invalid and the measurement will not be used
 */
#define MIN_VALID_RSSI -90
/**
 * @brief Maximum value for RSSI
 * Maximum valid value for RSSI measurement, if values goes over this it becomes
 * invalid and the measurement will not be used 
 */
#define MAX_VALID_RSSI -25
/**
 * @brief Values measured at one point.
 * This struct is used to store the measured values for encoder,
 * RSSI at delta (phaseshifted) and RSSI for zigma(phase) at each measuring point.
 * 
 */
typedef struct data_s
{
    int16_t encoder;
    int16_t delta;
	int16_t zigma;
} matrix_x3;

/**
 * @brief Encoder values for zero-points
 * This struct stores the encoder values for the zero-points.
 * 
 */
typedef struct data_a
{
    int16_t azimuth;
    int16_t elevation;
} zeros;

/**
 * @brief Sends measured RSSI signal into array
 * This function is used by "Observer.c" to send measured RSSI value
 * to either array for delta measurements or zigma measurements (data_delta and data_zigma).
 * The array is later used by "get_average" calculate the average value for a more specific reading.
 * 
 * @param rssi Value thats gone through Kalman filter
 * @param index Index for where to store in the array
 * @param state 0 for delta and 1 for zigma
 */
void send_data(int16_t rssi, int index, int state);

/**
 * @brief Set the average counter object
 * This function is used to set how many RSSI values should be measured before calculating
 * the average RSSI value. If counter has a higher cap on 10 and lower cap on 1.
 * @param value Counter value
 */
void set_average_counter(int16_t value);

/**
 * @brief Get the average object
 * This function is used to get the average RSSI value from array of RSSI values.
 * @param list List of RSSI values
 * @return Average value from list input 
 */
int16_t get_average(int16_t list[]);

/**
 * @brief Get the data object
 * This function takes in a "matrix_x3" and transfers the current stored Delta, Zigma and
 * encoder value into the variable.
 * @param buffer_data A pointer to a "matrix_x3" variable
 * @param N 0 for Azimuth and 1 for Elevation
 */
void get_data(matrix_x3 *buffer_data, int N);

/**
 * @brief Validates the measured values
 * This function checks the array consist of measured values are either over maximum set value
 * "MAX_VALID_RSSI" or under "MIN_VALID RSSI". If the values are outside the allowed are
 * the array at the index is set to 0.
 * @param raw_data A pointer to array with measured values
 * @param n A pointer to size of the array
 */
void value_validater(matrix_x3 *raw_data, int16_t *n);

/**
 * @brief Updates array
 * This function checks if some of the indexes of the array is set to 0. It will then
 * delete that index and update the size of the variable n.
 * 
 * @param data A pointer to array with measured values
 * @param n A pointer to size of the array
 */
void update_matrix(matrix_x3 *data, int16_t *n);

/**
 * @brief Checks if the measured point is an valid null-point
 * This function checks if the Delta value is smaller than the Zigma value and if the
 * Zigma value is smaller than the "ZIGMA_ZERO_VALUE". Returns true if the conditions 
 * are met and false else.
 * 
 * @param value_zigma Zigma RSSI value
 * @param value_delta Delta RSSI value
 * @param ZIGMA_ZERO_VALUE 
 * @return true If the condition are met
 * @return false If the conditions ar enot met
 */
bool zero_point_validater(int16_t value_zigma, int16_t value_delta, int16_t ZIGMA_ZERO_VALUE);

/**
 * @brief Finds the encoder zero point value of search
 * This function starts with calling "find_zigma_zero_value". Next it will find the first valid zero-point
 * and go through the rest of the array to find and return the lowest valid zero-point.
 * @param validated_values Array of measured values
 * @param n Size of array
 * @return index of zero-point
 */
int find_zero_point(matrix_x3 validated_values[], int n);

/**
 * @brief Finds new ZIGMA_ZERO_VALUE
 * This function takes in a array of measured values and finds the highest value for Zigma that are
 * used to verify the zero-point. It will subtract the highest value by 3 and return this value
 * @param values Array of measured values
 * @param n Size of array
 * @return int16_t Modified ZIGMA_ZERO_VALUE
 */
int16_t find_zigma_zero_value(matrix_x3 values[], int n);

