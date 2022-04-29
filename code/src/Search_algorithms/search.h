#include "observer.h"

/**
 * @brief Search and stores RSSI values within given area
 * This function changes the value for servo angle variable that the Threads
 * use to move the servo. Further the function activates the logic in the 
 * observer handler and waits until the semaphore is released from the handler
 * before it gets and transfer the data to readings array. It does this for the
 * whole sweep sector before releasing the semaphore.
 * 
 * @param state 0 for azimuth, 1 for Elevation
 * @param min_encoder_search Minimum search degrees for search sector
 * @param max_encoder_search Maximum search degrees for search sector
 * @param increment Value for increment for degrees
 */
void sweep_search(int state, int16_t min_encoder_search, int16_t max_encoder_search, int increment);

/**
 * @brief Get the readings object
 * This function is used to validate, update and retrieve the readings that were
 * done by sweep_search before resetting the array "readings"
 * @param main_readings Pointer to where the array where values are being stored
 * @param n Pointer to size of array
 * @return int 0 when successfull
 */
int get_readings(matrix_x3 *main_readings, int16_t *n);

/**
 * @brief Fine search of RSSI values
 * This function starts by calculating the new min_encoder_value and max_encoder_value
 * based on the zero_point and the sweep_sector. Further the function sets the 
 * average counter to 10 to get more accurate readings.
 * Then it calls on "sweep_search" with the new calculated parameters, gets the 
 * measurements and finds the zero-point index. 
 * The readings are then analysed for finding a new sweeps ector based on the delta value
 * at the zero-point index and threashold_degrees.
 * Depending if the new sweep sector is under threshold_search the function either exits
 * or calls upon itself (recursion) with the new parameters calculated
 * 
 * 
 * @param state 0 for Delta 1 for Zigma
 * @param threshold_degrees Threshold for RSSI value when defining new sweep sector
 * @param threshold_search Threshold for exiting function
 * @param sweep_sector Value for sweep sector
 * @param zero_point Encoder value for zero-point
 * @return int16_t Encoder value for zero-point
 */
int16_t fine_sweeper(int state, int threshold_degrees, int threshold_search, int sweep_sector, int16_t zero_point);

/**
 * @brief Sets values to 0
 * This function set all the values of the array "readings" to 0
 * @return zeros 0 when successful
 */
void reset_readings();


/**
 * @brief Coarse search
 * * This function starts search in azimuth plane by calling upon the "sweep_search" and uses
 * "find_zero_point" to find the encoder value for the zero-point. After finding the zero-point
 * it uses "validate_servo_zero_moved" to validate that the servomotor is at zero-point.
 * If SEARCH_PLANE is defined to be higher than 1 it starts by turning the antenna 90 degrees and 
 * searches in the elevation plane. 
 * When switching search plane the thread and encoder that is not being searched are disabled.
 * 
 * 
 * @return zeros Encoder values for zero-point in azimuth and elevation
 */
zeros coarse_search();

/**
 * @brief Fine search
 * This function starts by calling upon "fine_sweeper" with the parameters given and finds the
 * encoder values for the zero-point before it validates that the servo is at zero-point. 
 * If SEARCH_PLANE is defined to be higher than 1 it starts by turning the antenna 90 degrees and 
 * searches in the elevation plane.
 * When switching search plane the thread and encoder that is not being searched are disabled.
 * @param enc_values Zero-point values for azimuth and elevation plane
 * @return zeros Encoder values for zero-point in azimuth and elevation
 */
zeros fine_search(zeros enc_values);

/**
 * @brief Validates servomotor position
 * This function checks if the servomotor is at angle given from parameters.
 * It is used to wait for the motor is at correct position before the code starts searching in 
 * another plane.
 * 
 * @param N 0 for azimuth servomotor, 1 for elevation servomotor
 * @param zero_point_servo_angle Value for servomotor angle
 */
void validate_servo_zero_moved(int N, uint32_t zero_point_servo_angle);

/**
 * @brief Moves azimuth servo
 * Thread that is used to move servomotor for azimuth plane
 * 
 * @param azimuth_thread_servo_angle 
 */
void azimuth_servo_thread(uint32_t *azimuth_thread_servo_angle);

/**
 * @brief Moves elevation servo
 * Thread that is used to move servomotor for elevation plane
 * 
 * @param elevation_thread_servo_angle 
 */
void elevation_servo_thread(uint32_t *elevation_thread_servo_angle);