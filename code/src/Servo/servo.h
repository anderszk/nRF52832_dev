#include <zephyr.h>
#include <logging/log.h>
#include <drivers/gpio.h>

/**
 * @brief N for azimuth servomotor 
 */
#define servo_azimuth_N 0
/**
 * @brief Pin for azimuth servomotor 
 */
#define servo_azimuth_pin 2
/**
 * @brief N for elevation servomotor 
 */
#define servo_elevationl_N 1
/**
 * @brief Pin for elevation servomotor 
 */
#define servo_elevation_pin 3
/**
 * @brief N for antenna servomotor 
 */
#define servo_antenna_N 2
/**
 * @brief Pin for antenne servomotor
 */
#define servo_antenna_pin 4

/**
 * @brief Init timer
 * 
 */
int timer_init();

/**
 * @brief Start timer
 * 
 */
int timer_start();

/**
*@brief Funciton that initializes servo on specified pin. IMPORTANT, for servos to behave correctly,
* all prior servos N have to be initialized, and in order. That is, if initializing servo N = 1, 
* servo N = 0 has to also be initialized first.
*
* Takes in two parameters
* @param N Servo numer, from 0 to 3
* @param servo_pin Pin where servo is connected
*/
int servo_init(uint32_t N, int servo_pin);

/**
 * @brief Move servo to angle given in degrees
 * 
 * @param N Servo to be moved
 * @param angle Angle given in degrees
 */
void angle_move_servo(int N, uint32_t angle);

/**
 * @brief Move servo to position in ticks
 * 
 * Raw position is given in ticks, in this case the timer is running at a 
 * frequency of 16MHz. We have a counter modulus, TIMER_RELOAD of 320000. This
 * was found by dividing clock speed with servo frequency.
 * If our servo needs input frequency of 50Hz, we divide 50^-1 by 320000 and
 * get the period counter. If we know the dudy cycle of the servo, we can
 * calculate the input needed to position the servo with:
 *      W = duty_cycle/period_counter
 * For example, with neutral position in 1.5ms, we have:
 *      W = 0.0015/6.25*10^-8 = 24000
 * 
 * Specs for MG90S, 0.5ms to 2.5ms, neutral at 1.5, 0deg to 180deg
 * Specs for SER0038, 0.5ms = 0deg, 2.5ms = 270deg, 1.5 neutral
 * 
 * @param N Servo to be moved
 * @param position Position in ticks
 */
void raw_move_servo(int N, uint32_t position);


uint32_t sin_scaled(uint32_t input, uint32_t input_max, uint32_t output_min, uint32_t output_max);

/**
 * @brief Increments servo angle
 * Depending on the parameter N the function will update the servomotor angle and
 * use the function "raw_move_servo" and "convert_to_raw" to increment the servomotor angle
 * @param N 0 for azimuth, 1 for elevation
 */
void increment_servo(int N);

/**
 * @brief 
 * Depending on the parameter N the function will update the servomotor angle and
 * use the function "raw_move_servo" and "convert_to_raw" to decrement the servomotor angle
 * @param N 0 for azimuth, 1 for elevation
 */
void decrement_servo(int N);

/**
 * @brief Get the servo angle object
 * Depending on the parameter N the function will return either the servomotor angle
 * for either azimuth or elevation
 * @param N 0 for azimuth, 1 for elevation
 * @return int16_t Servomotor angle
 */
int16_t get_servo_angle(int N);



