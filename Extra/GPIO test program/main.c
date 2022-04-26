/*  --- Basic program for testing GPIO capabilities of the nRF52DK --- */

/*
    -   LED1 - LED4 configured as outputs on standard pins.

    -   BTN1 configured with polling to trigger LED1 on standard pin.

    -   BTN2 - BTN4 configured with interrupt to toggle LED2 - LED4 on standard pin.

    -   Three servo motors configured on pins 2, 3, and 4 with different
        angles to confirm correct behaviour of servo pins.
    
    -   Laser pointer configured on pin 28 in ON state.

    -   Switch configured on pin 27 to toggle on and off.

    -   Encoders will work if program runs without errors.

*/

#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>
#include <sys/printk.h>
#include "misc.h"
#include "servo.h"

#define LED_FLAGS       DT_GPIO_FLAGS(DT_ALIAS(led0), gpios)
#define BTN_FLAGS       DT_GPIO_FLAGS(DT_ALIAS(sw0), gpios)

#define SERVO1_PIN      2   // Servo pins could possibly be changed with encoder if encoders interfere with RF signal.
#define SERVO2_PIN      3
#define SERVO3_PIN      4

#define BTN1_PIN        13
#define BTN2_PIN        14
#define BTN3_PIN        15
#define BTN4_PIN        16

#define LED1_PIN        17
#define LED2_PIN        18
#define LED3_PIN        19
#define LED4_PIN        20

#define ENC1_CH_A       21  // Check if it collides with RESET function when used with encoder
#define ENC1_CH_B       22  // Encoders may have a problem with interfering with radio reciever if the frequency is too high,
#define ENC2_CH_A       23  // concider changing to pins other than P0.22 - P0.31, e.g. change with servo.
#define ENC2_CH_B       24

#define EXT_BTN1        25
#define EXT_BTN2        26

#define SWITCH_PIN      27
#define LASER_PIN       28

// Setting up interrupt handlers
void button2_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
	gpio_pin_toggle(dev, LED2_PIN);
}
void button3_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
	gpio_pin_toggle(dev, LED3_PIN);
}
void button4_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
	gpio_pin_toggle(dev, LED4_PIN);
}

// Setting up gpio_callback data structs
static struct gpio_callback button2_cb_data;
static struct gpio_callback button3_cb_data;
static struct gpio_callback button4_cb_data;

void main(void)
{
    const struct device *dev;

    int ret;
    int counter = 0;

    // Init servo
    timer_init();
    servo_init(0, SERVO1_PIN);
    servo_init(1, SERVO2_PIN);
    servo_init(2, SERVO3_PIN);
    timer_start();

    dev =  device_get_binding("GPIO_0");
    if(dev == NULL)
    {
        printk("Failed\n");
        return;
    }
    
    // Configure LEDs
    gpio_pin_configure(dev, LED1_PIN, GPIO_OUTPUT | GPIO_ACTIVE_HIGH);
    gpio_pin_configure(dev, LED2_PIN, GPIO_OUTPUT | LED_FLAGS);
    gpio_pin_configure(dev, LED3_PIN, GPIO_OUTPUT | LED_FLAGS);
    gpio_pin_configure(dev, LED4_PIN, GPIO_OUTPUT | LED_FLAGS);

    // Configure BTNs as inputs
    gpio_pin_configure(dev, BTN1_PIN, GPIO_INPUT | BTN_FLAGS);
    gpio_pin_configure(dev, BTN2_PIN, GPIO_INPUT | BTN_FLAGS);
    gpio_pin_configure(dev, BTN3_PIN, GPIO_INPUT | BTN_FLAGS);
    gpio_pin_configure(dev, BTN4_PIN, GPIO_INPUT | BTN_FLAGS);
    
    // Set up interrupt for BTNs with interrupt
    gpio_pin_interrupt_configure(dev, BTN2_PIN, GPIO_INT_EDGE_TO_ACTIVE | GPIO_INT_DEBOUNCE);
    gpio_pin_interrupt_configure(dev, BTN3_PIN, GPIO_INT_EDGE_TO_ACTIVE | GPIO_INT_DEBOUNCE);
    gpio_pin_interrupt_configure(dev, BTN4_PIN, GPIO_INT_EDGE_TO_ACTIVE | GPIO_INT_DEBOUNCE);

    // Helper for button iterrupt struct
	gpio_init_callback(&button2_cb_data, button2_pressed, BIT(14));
	gpio_init_callback(&button3_cb_data, button3_pressed, BIT(15));
	gpio_init_callback(&button4_cb_data, button4_pressed, BIT(16));

    // Add the callback for BTNs
	gpio_add_callback(dev, &button2_cb_data);
    gpio_add_callback(dev, &button3_cb_data);
    gpio_add_callback(dev, &button4_cb_data);

    // Configure external buttons
    ret = gpio_pin_configure(dev, EXT_BTN1, GPIO_INPUT | BTN_FLAGS);
    if (ret != 0)
    {
        printk("Error in external buttons");
        return;
    }
    ret = gpio_pin_configure(dev, EXT_BTN2, GPIO_INPUT | BTN_FLAGS);
    if (ret != 0)
    {
        printk("Error in external buttons");
        return;
    }

    // Configure encoder inputs
    ret = gpio_pin_configure(dev, ENC1_CH_A, GPIO_INPUT | BTN_FLAGS);
    if (ret != 0)
    {
        printk("Error in external buttons");
        return;
    }
    ret = gpio_pin_configure(dev, ENC1_CH_B, GPIO_INPUT | BTN_FLAGS);
    if (ret != 0)
    {
        printk("Error in external buttons");
        return;
    }
    ret = gpio_pin_configure(dev, ENC2_CH_A, GPIO_INPUT | BTN_FLAGS);
    if (ret != 0)
    {
        printk("Error in external buttons");
        return;
    }
    ret = gpio_pin_configure(dev, ENC2_CH_B, GPIO_INPUT | BTN_FLAGS);
    if (ret != 0)
    {
        printk("Error in external buttons");
        return;
    }

    //Configure misc
    laser_init(LASER_PIN);
    laser_set(1);
    gpio_pin_configure(dev, SWITCH_PIN, GPIO_OUTPUT_LOW);

    printk("Started...\n");

    while (1)
    {
        counter++;

        if (counter % 30 == 0)
        {
            gpio_pin_toggle(dev, SWITCH_PIN);
            counter = 0;
        }

        angle_move_servo(0, 0);
        angle_move_servo(1, 90);
        angle_move_servo(2, 180);

        // BTN 1 polling test 
        (gpio_pin_get(dev, BTN1_PIN) == 1) ? gpio_pin_set(dev, LED1_PIN, 1) : gpio_pin_set(dev, LED1_PIN, 0);

        k_msleep(50);
    }
}