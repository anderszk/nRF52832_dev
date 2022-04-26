#include "misc.h"

static int _laser_pin;

extern const struct device *dev;

void laser_init(int laser_pin)
{
    //Dev er allerede definert så vi gjør den heller global
    // dev = device_get_binding("GPIO_0");
    // if (dev == NULL)
    // {
    //     printk("Laser init failed.\n");
    //     return;
    // }

    gpio_pin_configure(dev, laser_pin, GPIO_OUTPUT_LOW);

    _laser_pin = laser_pin;

    printk("Laser init successfull.\n");
}

void laser_toggle()
{
    gpio_pin_toggle(dev, _laser_pin);
}

void laser_set(bool state)
{
    gpio_pin_set(dev, _laser_pin, state);
}