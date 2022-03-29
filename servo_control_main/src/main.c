#include <zephyr.h>
#include <sys/printk.h>
#include "servo_control.h"
#include <math.h>

//Define servos
#define SERVO_ASIMUT 	{0, 13}
#define SERVO_ELEVATION {1, 14}
#define SERVO_ROTATE 	{2, 15}
#define LED_4 			{3, 16}

static uint32_t servo_asimut[]		= SERVO_ASIMUT;
static uint32_t servo_elevation[]	= SERVO_ELEVATION;
static uint32_t servo_rotate[]		= SERVO_ROTATE;
static uint32_t led_4[]				= LED_4;

uint32_t sin_scaled(uint32_t input, uint32_t input_max, uint32_t output_min, uint32_t output_max)
{
    float sin_val = sin((float)input * 2.0f * 3.141592f / (float)input_max);
    return (uint32_t)(((sin_val + 1.0f) / 2.0f) * (float)(output_max - output_min)) + output_min; 
}

void main(void)
{
	printk("HELLO??\n");

	//Define counter
	uint32_t counter = 0;

	//Initialize timer
	timer_init();

	// Initializing servo motors
	servo_init(servo_asimut[0], servo_asimut[1]);
	servo_init(servo_elevation[0], servo_elevation[1]);
	

	//Start timer
	timer_start();

	for(;;)
	{
		for (int i = 0; i < 270; i+=27)
		{
			for (int j = i; j <= i+27; j++)
			{
				counter++;
				angle_move_servo(servo_asimut[0], sin_scaled(counter, 270, 0, 270));
				k_sleep(K_MSEC(15));
			}
			k_sleep(K_MSEC(300));
		}
		

		// for(int i = 50; i < 120; i++){
		// 	angle_move_servo(servo_asimut[0], i);
		// 	k_sleep(K_MSEC(100));
		// }
		// for(int i = 120; i >50; i--){
		// 	angle_move_servo(servo_asimut[0], i);
		// 	k_sleep(K_MSEC(100));
		// }
		// for(int i = 50; i < 120; i++){
		// 	angle_move_servo(servo_asimut[0], i);
		// 	k_sleep(K_MSEC(75));
		// }
		// for(int i = 120; i >50; i--){
		// 	angle_move_servo(servo_asimut[0], i);
		// 	k_sleep(K_MSEC(75));
		// }

		// angle_move_servo(servo_asimut[0], 20);
		// k_sleep(K_MSEC(500));
		// angle_move_servo(servo_asimut[0], 150);
		// k_sleep(K_MSEC(500));
		// angle_move_servo(servo_asimut[0], 20);
		// k_sleep(K_MSEC(500));
		// angle_move_servo(servo_asimut[0], 20);
		// k_sleep(K_MSEC(500));
		// angle_move_servo(servo_asimut[0], 20);
		// k_sleep(K_MSEC(500));
	}
}