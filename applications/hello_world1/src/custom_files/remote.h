#include <zephyr.h>
#include <logging/log.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>
#include <bluetooth/hci.h>


#define BT_UUID_REMOTE_SERV_VAL \
   BT_UUID_128_ENCODE(0x209f0001,0x5385,0x4cea,0xb40b,0x71096eb1298c)
   

#define BT_UUID_REMOTE_SERVICE  BT_UUID_DECLARE_128(BT_UUID_REMOTE_SERV_VAL)

#define BT_UUID_REMOTE_BUTTON_CHRC_VAL \
	BT_UUID_128_ENCODE(0x209f0002,0x5385,0x4cea,0xb40b,0x71096eb1298c)

#define BT_UUID_REMOTE_BUTTON_CHRC 	BT_UUID_DECLARE_128(BT_UUID_REMOTE_BUTTON_CHRC_VAL)

#define BT_UUID_REMOTE_BUTTON_MESSAGE_VAL \
   BT_UUID_128_ENCODE(0x209f0003,0x5385,0x4cea,0xb40b,0x71096eb1298c)

#define BT_UUID_REMOTE_MESSAGE_CHRC BT_UUID_DECLARE_128(BT_UUID_REMOTE_BUTTON_MESSAGE_VAL)

enum bt_button_notifications_enabled {
	BT_BUTTON_NOTIFICATIONS_ENABLED,
	BT_BUTTON_NOTIFICATIONS_DISABLED,
};

struct bt_remote_service_cb {
	void (*notif_changed)(enum bt_button_notifications_enabled status);
	void (*data_received)(struct bt_conn *conn, const uint8_t *const data, uint16_t len);
};


int bluetooth_init(struct bt_conn_cb * bt_cb, struct bt_remote_service_cb *remote_cb);
void set_button_value(uint8_t button_pressed);
int send_button_notification(struct bt_conn *conn, uint8_t value);
