#include "initiater.h"


int initiate_modules(){
    int err;
   	err = configure_dk_buttons_leds();
	if (err){
		return err;
	}
	err = timer_init();
	err = timer_start();
	err = init_bluethooth_scan();
	err = init_encoder_servos(); 
	set_average_counter(1);
	err = init_encoder_azimuth();
	
	// laser_init(15);


    return err;
}