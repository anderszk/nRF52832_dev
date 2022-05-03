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
int laser_init();

/**
 * @brief Toggles laser pointer on or off
 * 
 */
void laser_toggle(void);

/**
 * @brief Set laser to state.
 * 
 * @param state int, 0 for on, 1 for off.
 */
void laser_set(int state);