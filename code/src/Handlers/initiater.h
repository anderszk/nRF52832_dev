// #include "button_leds.h"
#include "buttons.h"
/**
 * @brief Initiate necessary modules
 * This function initiates buttons, leds, timer, BLE, servomotors, laser and azimuth encoder.
 * Sets the average_counter to 1 and turns of the laser. If an error occurs while initiating a module
 * it will print and return the error code and exit the function.
 * 
 * @return int 0 when successful
 */

int initiate_modules();
