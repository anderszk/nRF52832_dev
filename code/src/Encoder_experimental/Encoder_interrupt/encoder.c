#include "encoder.h"
int enc_1_counter = 0;
int enc_2_counter = 0;

void enc1_a_trigger(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    enc_1_counter++;
    printk("Position %i\n", enc_1_counter);
}

void enc1_b_trigger(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{

}

static struct gpio_callback button1_cb_data;
static struct gpio_callback button2_cb_data;

void encoder_init(encoder *enc_1, encoder *enc_2)
{
    const struct device *dev;

    int ret;

    dev = device_get_binding("GPIO_0");
    if (dev == NULL)
    {
        printk("Encoder init failed.\n");
    }
    
    ret = gpio_pin_interrupt_configure(dev, enc_1->ch_a, GPIO_INT_EDGE_BOTH);
    ret = gpio_pin_interrupt_configure(dev, enc_1->ch_b, GPIO_INT_EDGE_BOTH);

    gpio_init_callback(&button1_cb_data, enc1_a_trigger, BIT(enc_1->ch_a));
    gpio_init_callback(&button2_cb_data, enc1_b_trigger, BIT(enc_1->ch_b));

    gpio_add_callback(dev, &button1_cb_data);
    gpio_add_callback(dev, &button2_cb_data);
}

void encoder_get_value(encoder *enc_n)
{
    if (enc_n->enc_n == 0)
    {
        printk("Encoder 1 count: %i\n", enc_1_counter);
    }
    if (enc_n->enc_n == 1)
    {
        printk("Encoder 2 count: %i\n", enc_2_counter);
    }
}