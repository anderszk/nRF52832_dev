#include <bluetooth/bluetooth.h>
#include "data_processor.h"

/**
 * @brief Adds address to filter
 * This function takes in an address and adress type in form of a string and 
 * converts this into an ble_address. The address is then added into the filter
 * accept list. 
 * 
 * @param addr_str Address in form of a string
 * @param type Type in form of a string ("random") or ("public")
 * @return int 0 if successful
 */

int add_filter_accept_list_from_string(const char *addr_str,const char *type);

/**
 * @brief Initiates bluethooth
 * This function starts by defining the parameters for the observer handler,
 * setting the device tree to "GPIO_0", further the function enables bluethooth,
 * adds address to filter list, initiates and starts the Observer handler and initiate the
 * GPIO used for the switch
 * 
 * @return int 0 if successful
 */
int init_bluethooth_scan();

/**
 * @brief Observer handler
 * The static variable counter and state (0 for Delta, 1 for Zigma) are defined.
 * When send_data_state is true this handler alternates sending the filtrated RSSI value to 
 * array for Delta and Zigma values depending on the value of "average_counter". When alternating
 * it sets the GPIO for the switch "0" for measuring in Zigma and "1" for measuring in Delta.
 * When the function has goes from Zigma measuring to Delta measuring it will free the semaphore "my_sem",
 * which is used to ensure that the observer has gotten both measurement in Delta and Zigma before the robot moves.
 * If the variable send_data_state is false the handler will not do anything when entering.
 * @param addr Adress of the observed signal
 * @param rssi RSSI value of the observed signal
 * @param type Address type of the observed signal
 * @param ad Add of the observed signal
 */
static void device_found(const bt_addr_le_t *addr, int8_t rssi, uint8_t type, struct net_buf_simple *ad);

/**
 * @brief Set the observer object
 * This function is being used by "sweep_search" to activate the logic inside the observer handler
 * @param state false or true
 */
void set_observer(bool state);










