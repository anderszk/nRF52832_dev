#include "observer.h"
#define switch_pin 29

extern int16_t average_counter;


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
		static int state = 0; // 0 = delta, 1 = zigma
		if(state == 0){
			rssi = KALMAN_DELTA(rssi);
			send_data(rssi, counter, state);
			counter +=1;
			if(counter >= average_counter){
				counter = 0;
				state = 1;
				gpio_pin_set(dev, switch_pin, 0);
			}
		}
		else if(state == 1){
			rssi = KALMAN_ZIGMA(rssi);
			send_data(rssi, counter, state);
			counter += 1;
			if(counter >= average_counter){
				state = 0;
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
		printk("Could not convert string to bluethooth address (error: %d).\n", err);
		return err;
	}
	err = bt_le_filter_accept_list_add(&addr_le);
	if (err){
		printk("Could not add address to acceptlist (error: %d).\n", err);
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
	printk("Starting Observer.\n");
	/* Initialize the Bluetooth Subsystem */
	err = bt_enable(NULL);
	if (err) {
		printk("Bluetooth init failed (error %d).\n", err);
		return err;
	}
    err = add_filter_accept_list_from_string("E7:9E:11:9E:33:EE", "(random)");
	if (err){
		printk("Could not add to acceptlist (error: %d).", err);
		return err;
	}    
	printk("Bluetooth initialized.\n");

	err = bt_le_scan_start(&scan_param, device_found);
	if (err) {
		printk("Starting scanning failed (err %d).\n", err);
		return err;
	}
	err = gpio_pin_configure(dev, switch_pin, GPIO_OUTPUT_ACTIVE | GPIO_ACTIVE_LOW);
	if (err) {
		printk("Could not configure pin for switch (err %d).\n", err);
		return err;
	}
	printk("Initializing switch on pin %d success!\n", switch_pin);

	gpio_pin_set(dev, switch_pin, 1);
	k_sem_give(&my_sem);
	return err;
}