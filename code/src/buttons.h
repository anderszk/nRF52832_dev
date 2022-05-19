#include <dk_buttons_and_leds.h>
#include <drivers/gpio.h>
#include "search.h"
#include "laser.h"

/**
 * @brief Configures buttons and leds
 * This function initiates the four leds and a button_handler which are connected to the buttons
 * on the DK. 
 * @return int 0 when successful
 */
int configure_dk_buttons_leds();
