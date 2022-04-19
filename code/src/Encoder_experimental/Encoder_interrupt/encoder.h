#include <zephyr.h>
#include <sys/printk.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>
#include "servo_control.h"

/**
 * @brief Struct holding information about encoder. Use this to initialize encoders.
 * 
 */
typedef struct Encoder
{
    /**
     * Encoder number, for identifying. Only two can be initialized so must be either 0 or 1.
     */
    int enc_n;
    /**
     * Pin connected to ch a of encoder N
     */
    int ch_a;
    /**
     * Pin connected to ch b of encoder N
     */
    int ch_b;
    /**
     * Pin conndected to index of encoder N
     */
    int index;
}encoder;

/**
 * @brief Initialize encoders. Takes in channel A and channel B of two encoders.
 * Set encoder 2 to NULL if only one is connected.
 * 
 * @param enc_a Struct that contains ch a, ch b and index pins of encoder 1
 * @param enc_b Struct that contains ch a, ch b and index pins of encoder 2
 */
void encoder_init(encoder *enc_1, encoder *enc_2);

/**
 * @brief Get value of encoder N.
 * 
 * @param enc_n Address of encoder struct.
 */
void encoder_get_value(encoder *enc_n);