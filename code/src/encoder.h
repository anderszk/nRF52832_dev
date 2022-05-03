#include <logging/log.h>
#include "servo.h"
#include <nrfx_qdec.h>


/**
 * @brief Initiates servos and encoder
 * This function initiates servomotor for azimuth plane, elevation plane and for the antenna before 
 * it moves the servomotors to starting positions. The function then resets the encoder values
 * and initialize the irq handler for qdec.
 * 
 * @return int 0 if successful
 */
int init_encoder_servos();

/**
 * @brief Initialize azimuth encoder
 * This function starts by disabling any current active encoders before initializing the encoder
 * given the parameters set. If no error occurs the encoder is enabled .
 * @return int NRFX_SUCCESS if successful
 */
int init_encoder_azimuth();

/**
 * @brief Initialize elevation encoder
 * This function starts by disabling any current active encoders before initializing the encoder
 * given the parameters set. If no error occurs the encoder is enabled .
 * @return int NRFX_SUCCESS if successful
 */
int init_encoder_elevation();

/**
 * @brief Updates encoder value
 * Depending on the parameter N the function will update the encoder value and convert it into
 * degrees (8192 pulses/360 degrees) = 23.
 * @param N 0 for azimuth and 1 elevation
 */
void update_encoder(int N);

/**
 * @brief Moves servomotor gradually
 * Depending on the parameter N the function will convert the input angle into the servomotors
 * raw angle. Further the function will call upon either "increment_servo" og "decrement_servo"
 * and update the encoder value with "update_encoder".
 * 
 * @param N 0 for azimuth servomotor, 1 for elevation servomotor
 * @param angle Working angles for the robot (0-180) azimuth, (0-70) elevation.
 */
void angle_slow_move(int N, uint32_t angle);

/**
 * @brief Get the encoder object
 * Depending on the parameter N the function will return the encoder values in degrees
 * @param N 0 for azimuth, 1 for elevation
 * @return int16_t Encoder value in degrees
 */
int16_t get_encoder(int N);

