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
 * Depending the parameter N the function will update the encoder value and convert it into
 * degrees (8192 pulses/360 degrees) = 23.
 * @param N 0 for azimuth and 1 elevation
 */
void update_encoder(int N);

/**
 * @brief Moves servomotor slowly
 * This function 
 * 
 * @param N 
 * @param angle 
 */
void angle_slow_move(int N, uint32_t angle);

/**
 * @brief Get the encoder object
 * 
 * @param N 
 * @return int16_t 
 */
int16_t get_encoder(int N);
/**
 * @brief 
 * 
 * @param event 
 */
