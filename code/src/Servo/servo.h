#include <zephyr.h>
#include <logging/log.h>

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
*@brief Funciton that initializes servo on specified pin.
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

//Increment servo

//Decrement servo

//Sweep motion, speed.

uint32_t sin_scaled(uint32_t input, uint32_t input_max, uint32_t output_min, uint32_t output_max);