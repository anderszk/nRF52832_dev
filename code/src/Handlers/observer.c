#include "observer.h"
#define LOG_MODULE_NAME Observer
LOG_MODULE_REGISTER(LOG_MODULE_NAME);

extern struct k_sem my_sem; 
bool send_data_state = false;

void set_observer(bool state){
	send_data_state = state;
}

void set_switch(int state){
	//set gpio switch to value;
}

static void device_found(const bt_addr_le_t *addr, int8_t rssi, uint8_t type,
			 struct net_buf_simple *ad)
{	
	if(send_data_state){
    	// rssi = KALMAN(rssi);
    	LOG_INF("%d",rssi);
		static int counter = 0;
		static bool state = false; // 0 = delta, 1 = zigma, 2 = done
		if(!state){
			send_data_delta(rssi, counter);
			counter +=1;
			if(counter > 4){
				counter = 0;
				state = true;
			}
		}
		else if(state){
			send_data_zigma(rssi, counter);
			counter += 1;
			if(counter > 4){
				state = false;
				counter = 0;
				set_observer(false);
				k_sem_give(&my_sem);
			}
		}

	}

	else{return;}





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
	int err;
	LOG_INF("Starting Observer\n");
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
	LOG_INF("Bluetooth initialized\n");

	err = bt_le_scan_start(&scan_param, device_found);
	if (err) {
		LOG_ERR("Starting scanning failed (err %d)\n", err);
		return err;
	}
	return err;
}