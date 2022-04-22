#include "encoder.h"

#define LOG_MODULE_NAME ENCODER
LOG_MODULE_REGISTER(LOG_MODULE_NAME);

#define servo_azimuth_N 0
#define servo_azimuth_pin 13
#define servo_horizontal_N 1
#define servo_horizontal_pin 15
#define servo_antenna_N 2
#define servo_antenna_pin 16

int16_t azimuth_encoder_value = 0;
int16_t horizontal_encoder_value = 0;
extern struct k_sem my_sem;



int16_t get_encoder(int N){//fake encoder verdier som gjenspeiler servo vinkelen så bra den kan
    if(N < 1){
        azimuth_encoder_value += 1;
        return azimuth_encoder_value;
    }
    else{
        horizontal_encoder_value += 1;
        return horizontal_encoder_value;
    }
}

int init_encoder(){
    //Masse encoder inits
	int err = servo_init(servo_azimuth_N, servo_azimuth_pin);
    err = servo_init(servo_horizontal_N,servo_horizontal_pin);
    err = servo_init(servo_antenna_N, servo_antenna_pin);
    angle_move_servo(servo_azimuth_N, 0);
    angle_move_servo(servo_horizontal_N, 25);
    angle_move_servo(servo_antenna_N, 0);
    // angle_move_servo(servo_azimuth_N, 90);
	// k_sem_take(&my_sem, K_FOREVER);
    // k_sem_take(&my_sem, K_FOREVER);
    // while(1){
    //     angle_slow_move(servo_horizontal_N ,25);
    //     k_msleep(2500);
    //     angle_slow_move(servo_horizontal_N,60);
    //     k_msleep(2500);
    // }

    
    return err;
}

void set_encoder(int value, int N){
    if(N < 1){
        azimuth_encoder_value = value;}
    else{horizontal_encoder_value = value;}
}