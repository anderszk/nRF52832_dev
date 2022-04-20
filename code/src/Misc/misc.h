#include <zephyr.h>
#include <sys/printk.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>

/**
 * @brief Initialize laser pointer
 * 
 * @param laser_pin Pin connected to laser +5v (nRF52 not compatible with 5v on gpio).
 */
void laser_init(int laser_pin);

/**
 * @brief Toggles laser pointer on or off
 * 
 */
void laser_toggle(void);

/**
 * @brief Set laser to state.
 * 
 * @param state Boolean, 1 for on, 0 for off.
 */
void laser_set(bool state);