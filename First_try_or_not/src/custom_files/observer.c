#include "observer.h"

#define SERVO_ASIMUT 	{0, 13}

static uint32_t servo_asimut[]		= SERVO_ASIMUT;
int asimuth_array[180];


static void device_found(const bt_addr_le_t *addr, int8_t rssi, uint8_t type,
			 struct net_buf_simple *ad)
{
	static uint32_t servoAngle = 0;
	char addr_str[BT_ADDR_LE_STR_LEN];

	bt_addr_le_to_str(addr, addr_str, sizeof(addr_str));
	rssi = KALMAN(rssi); 
	// printk("Device found: %s (Modified RSSI %d)\n", addr_str, rssi);
	asimuth_array[servoAngle] = rssi;
	angle_move_servo(servo_asimut[0],servoAngle);
	servoAngle ++;
	if(servoAngle == 180){
		servoAngle = 0;
		angle_move_servo(servo_asimut[0],servoAngle);
		for (int i = 0; i <180; i++){
			printk("%d", asimuth_array[i]);
			printk("\n");
		}
	}

	
	
}


void init_bluethooth_scan(){

	struct bt_le_scan_param scan_param = {
		.type       = BT_LE_SCAN_TYPE_ACTIVE,
		.options    = BT_LE_SCAN_OPT_FILTER_ACCEPT_LIST,
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


	const bt_addr_le_t addr_le = {.a = BT_ADDR_LE_ANY, .type = BT_ADDR_LE_RANDOM};
	const char *addr_str = "E4:A6:69:9D:F7:7F";
	char *addr_str2[27];
	err = bt_addr_le_from_str(addr_str,"(random)", &addr_le);
	if (err){
		printk("error: %d", err);
	}
	err = bt_addr_le_to_str(&addr_le,&addr_str2,sizeof(addr_str2));
	err = bt_le_filter_accept_list_add(&addr_le);
	if (err){
		printk("Could not add to acceptlist (error: %d)", err);
	}

	printk("address after: %s", &addr_str2);
	
	err = bt_le_scan_start(&scan_param, device_found);
	printk("\n");
	if (err) {
		printk("Starting scanning failed (err %d)\n", err);
		return;
	}
	servo_init(servo_asimut[0], servo_asimut[1]);

    
}
