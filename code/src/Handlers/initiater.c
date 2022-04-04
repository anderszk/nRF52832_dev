#include "initiater.h"


int initiate_modules(){
    int err;
   	// err = configure_dk_buttons_leds();
	err = timer_init();
	err = timer_start();
	err = init_bluethooth_scan();

    return err;
}