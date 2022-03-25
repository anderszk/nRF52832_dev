#include "button_leds.h"

LOG_MODULE_REGISTER(LOG_MODULE_NAME);

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)

#if DT_NODE_HAS_STATUS(LED0_NODE, okay)
#define LED0	DT_GPIO_LABEL(LED0_NODE, gpios)
#define PIN	DT_GPIO_PIN(LED0_NODE, gpios)
#define FLAGS	DT_GPIO_FLAGS(LED0_NODE, gpios)
#else
#define LED0	""
#define PIN	0
#define FLAGS	0
#endif

//LEDS
#define LOG_MODULE_NAME app
#define RUN_STATUS_LED DK_LED1
#define CONN_STATUS_LED DK_LED2
#define RUN_LED_BLINK_INTERVAL 1000

//GPIOS
#define outPin 10

//
const struct device *dev;

void button_handler(uint32_t button_state, uint32_t has_changed){
	
    int button_pressed = 0;
	if (has_changed & button_state){
		switch (has_changed){
			
			case DK_BTN1_MSK:
				button_pressed = 1;
                gpio_pin_set(dev,26,1);
				break;
			case DK_BTN2_MSK:
			    button_pressed = 2;
                gpio_pin_set(dev,26,0);
				break;
			case DK_BTN3_MSK:
				button_pressed = 3;
				break;
			case DK_BTN4_MSK:
				button_pressed = 4;
				break;
			default:
				break;
		}
		LOG_INF("Button %d pressed", button_pressed);

	}
}

void configure_dk_buttons_leds(){
	int err = dk_leds_init();
	if(err){
		LOG_ERR("Could not init LEDS(err %d",err);
	}

	err = dk_buttons_init(button_handler);
	if(err){
		LOG_ERR("Could not init BUTTONS(err %d", err);
	}

    dev = device_get_binding(LED0);
	if (dev == NULL) {
		return;
	}

    err = gpio_pin_configure(dev, PIN, GPIO_OUTPUT_ACTIVE | FLAGS);
	if (err) {
		LOG_ERR("Could not init gpio %d", PIN);
	}
    err = gpio_pin_configure(dev, 27, GPIO_OUTPUT_ACTIVE | FLAGS);
    if (err) {
		LOG_ERR("Could not init gpio %d", 27);
	}
    err = gpio_pin_configure(dev, 26, GPIO_OUTPUT_ACTIVE | FLAGS);
    if (err) {
		LOG_ERR("Could not init gpio %d", 26);
	}
    gpio_pin_set(dev,27,1);
    
    }
