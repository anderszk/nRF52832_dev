#include "buttons.h"
#define LOG_MODULE_NAME BUTTONS_LEDS

LOG_MODULE_REGISTER(LOG_MODULE_NAME);


/* The devicetree node identifier for the "led0" alias. */
// #define LED0_NODE DT_ALIAS(led0)

// #if DT_NODE_HAS_STATUS(LED0_NODE, okay)
// #define LED0	DT_GPIO_LABEL(LED0_NODE, gpios)
// #define PIN	DT_GPIO_PIN(LED0_NODE, gpios)
// #define FLAGS	DT_GPIO_FLAGS(LED0_NODE, gpios)
// #else
// #define LED0	""
// #define PIN	0
// #define FLAGS	0
// #endif

//LEDS
#define RUN_STATUS_LED DK_LED1
#define CONN_STATUS_LED DK_LED2
#define RUN_LED_BLINK_INTERVAL 1000

//GPIOS
#define outPin 10

//
extern const struct device *dev;

void button_handler(uint32_t button_state, uint32_t has_changed){
	
    int button_pressed = 0;
	if (has_changed & button_state){
		switch (has_changed){
			
			case DK_BTN1_MSK:
				button_pressed = 1;
                increment_servo(0);
				break;
			case DK_BTN2_MSK:
			    button_pressed = 2;
                decrement_servo(0);
				break;
			case DK_BTN3_MSK:
				button_pressed = 3;
                get_servo_angle();
				break;
			case DK_BTN4_MSK:
				button_pressed = 4;
				break;
			default:
				break;
		}
		printk("Button %d pressed\n", button_pressed);
		

	}
}

int configure_dk_buttons_leds(){
	int err = dk_leds_init();
	if(err){
		LOG_ERR("Could not init LEDS(err %d",err);
	}

	err = dk_buttons_init(button_handler);
	if(err){
		LOG_ERR("Could not init BUTTONS(err %d", err);
	}

    // dev = device_get_binding("GPIO_0");
	// if (dev == NULL) {
	// 	return;
	// }


	return err;
    
    }
