#include "initiater.h"


int initiate_modules(){
    int err;
   	err = configure_dk_buttons_leds();
	if (err){
		printk("Error while configuring buttons and leds (error: %d).\n", err);
		return err;
	}
	err = timer_init();
	if (err){
		printk("Error while configuring timer (error: %d).\n", err);
		return err;
	}
	err = timer_start();
	if (err){
		printk("Error while starting timer (error: %d).\n", err);
		return err;
	}
	err = init_bluethooth_scan();
	if (err){
		printk("Error while configuring bluethooth and observer (error: %d).\n", err);
		return err;
	}
	err = init_encoder_servos(); 
	if (err){
		printk("Error while configuring servomotors (error: %d).\n", err);
		return err;
	}
	err = init_encoder_azimuth();
	if (err != NRFX_SUCCESS){
		printk("Error while configuring Azimuth encoder (error: %d).\n", err);
		return err;
	}
	err = laser_init(28);
	if (err){
		printk("Error while configuring laser (error: %d).\n", err);
		return err;
	}
	set_average_counter(1);	
	laser_set(1);


    return err;
}