#include "buttons.h"


extern struct k_sem my_sem;
extern const struct device *dev;

void button_handler(uint32_t button_state, uint32_t has_changed){
	static int N = 0;
	
    int button_pressed = 0;
	if (has_changed & button_state){
		switch (has_changed){
			case DK_BTN1_MSK:
				button_pressed = 1;
                increment_servo(N);
				// laser_set(0);
				break;
			case DK_BTN2_MSK:
			    button_pressed = 2;
                decrement_servo(N);
				// laser_set(1);
				break;
			case DK_BTN3_MSK:
				button_pressed = 3;
                printk("Angle: %d\n",get_servo_angle(N));
				break;
			case DK_BTN4_MSK:
				button_pressed = 4;
				// k_sem_give(&my_sem);
				if(N == 0){
					N = 1;
					init_encoder_elevation();
				}
				else {
					N = 0;
					init_encoder_azimuth();
				}
				break;
			default:
				break;
		}
		printk("Button %d pressed.\n", button_pressed);
		

	}
}

int configure_dk_buttons_leds(){
	int err = dk_leds_init();
	if(err){
		printk("Could not init LEDS (error: %d).",err);
		return err;
	}

	err = dk_buttons_init(button_handler);
	if(err){
		printk("Could not init BUTTONS (error: %d).", err);
		return err;
	}
	printk("Buttons and leds initiated.\n");
	return err;
}
    
    
