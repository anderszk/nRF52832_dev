#include "encoder.h"

#define LOG_MODULE_NAME ENCODER
LOG_MODULE_REGISTER(LOG_MODULE_NAME);

#define servo_azimuth_N 0
#define servo_azimuth_pin 13

int16_t azimuth_encoder_value = 0;


void test_me_3(){
    LOG_INF("dsadsahehhehehe");
}

int16_t get_encoder(){//fake encoder verdier som gjenspeiler servo vinkelen så bra den kan
    
    azimuth_encoder_value += 1;
    return azimuth_encoder_value;
}

int init_encoder(){
    //Masse encoder inits
	int err = servo_init(servo_azimuth_N, servo_azimuth_pin);
    
    return err;
}

void set_encoder(int value){
    azimuth_encoder_value = value;
}