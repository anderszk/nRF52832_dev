#include <bluetooth/bluetooth.h>
#include "kalman_filter.h"
#include "data_processor.h"


int add_filter_accept_list_from_string(const char *addr_str,const char *type);
int init_bluethooth_scan();
static void device_found(const bt_addr_le_t *addr, int8_t rssi, uint8_t type, struct net_buf_simple *ad);
void set_observer(bool state);
void set_switch(int state);








