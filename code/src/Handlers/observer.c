#include "observer.h"
#define LOG_MODULE_NAME Observer
#define switch_pin 14

extern int16_t average_counter;

LOG_MODULE_REGISTER(LOG_MODULE_NAME);

extern struct k_sem my_sem; 
const struct device *dev;
bool send_data_state = false;

void set_observer(bool state){
	send_data_state = state;

}


static void device_found(const bt_addr_le_t *addr, int8_t rssi, uint8_t type,
			 struct net_buf_simple *ad)
{	
	if(send_data_state){
    	
		static int counter = 0;
		static bool delta_zigma_state = false; // 0 = delta, 1 = zigma
		if(!delta_zigma_state){
			rssi = KALMAN(rssi);
			send_data_delta( rssi, counter);
			counter +=1;
			if(counter >= average_counter){
				counter = 0;
				delta_zigma_state = true;
				gpio_pin_set(dev, switch_pin, 0);
			}
		}
		else if(delta_zigma_state){
			rssi = KALMAN_ZIG(rssi);
			send_data_zigma(rssi, counter);
			counter += 1;
			if(counter >= average_counter){
				delta_zigma_state = false;
				counter = 0;
				gpio_pin_set(dev, switch_pin, 1);
				set_observer(false);
				k_sem_give(&my_sem);
				
			}
		}

	}
	// else{printk("rssi: %d\n", KALMAN(rssi));}
}


int add_filter_accept_list_from_string(const char *addr_str,const char *type){
    int err;
	bt_addr_le_t addr_le = {.a = BT_ADDR_LE_ANY, .type = BT_ADDR_LE_RANDOM};
	err = bt_addr_le_from_str(addr_str, type, &addr_le);
	if (err){
		LOG_ERR("error: %d", err);
	}
	err = bt_le_filter_accept_list_add(&addr_le);
	if (err){
		LOG_ERR("Could not add to acceptlist (error: %d)", err);
		return err;
	}
    return err;
}


int init_bluethooth_scan(){

	struct bt_le_scan_param scan_param = {
		.type       = BT_LE_SCAN_TYPE_ACTIVE,
		.options    = BT_LE_SCAN_OPT_FILTER_ACCEPT_LIST,
		.interval   = BT_GAP_SCAN_FAST_INTERVAL,
		.window     = BT_GAP_SCAN_FAST_WINDOW,
	};

	dev = device_get_binding("GPIO_0");
	if (dev == NULL) {
		return;
	}
	int err;
	printk("Starting Observer\n");
	/* Initialize the Bluetooth Subsystem */
	err = bt_enable(NULL);
	if (err) {
		LOG_ERR("Bluetooth init failed (err %d)\n", err);
		return err;
	}
    err = add_filter_accept_list_from_string("C8:4A:E6:23:F5:13", "(random)");
	if (err){
		LOG_ERR("Could not add to acceptlist (error: %d)", err);
		return err;
	}    
	printk("Bluetooth initialized\n");

	err = bt_le_scan_start(&scan_param, device_found);
	if (err) {
		LOG_ERR("Starting scanning failed (err %d)\n", err);
		return err;
	}
	err = gpio_pin_configure(dev, switch_pin, GPIO_OUTPUT_ACTIVE | GPIO_ACTIVE_LOW);
	if (err) {
		LOG_ERR("Configuring pin failed (err %d)\n", err);
		return err;
	}
	gpio_pin_set(dev, switch_pin, 1);
	k_sem_give(&my_sem);
	return err;
}