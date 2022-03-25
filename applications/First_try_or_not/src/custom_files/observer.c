#include "observer.h"

static void device_found(const bt_addr_le_t *addr, int8_t rssi, uint8_t type,
			 struct net_buf_simple *ad)
{
	char addr_str[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(addr, addr_str, sizeof(addr_str));
	if(!strcmp(addr_str,"E4:A6:69:9D:F7:7F (random)")){
		printk("Device found: %s (RSSI %d)\n", addr_str, rssi);
	
		rssi = KALMAN(rssi); 
		printk("Device found: %s (Modified RSSI %d)\n", addr_str, rssi);
	}
	
}


void init_bluethooth_scan(){

	struct bt_le_scan_param scan_param = {
		.type       = BT_LE_SCAN_TYPE_PASSIVE,
		.options    = BT_LE_SCAN_OPT_FILTER_DUPLICATE,
		.interval   = BT_GAP_SCAN_FAST_INTERVAL,
		.window     = BT_GAP_SCAN_FAST_WINDOW,
	};
	int err;

	printk("Starting Observer\n");

	/* Initialize the Bluetooth Subsystem */
	err = bt_enable(NULL);
	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
		return;
	}

	printk("Bluetooth initialized\n");

	err = bt_le_scan_start(&scan_param, device_found);
	printk("\n");
	if (err) {
		printk("Starting scanning failed (err %d)\n", err);
		return;
	}
    
}
