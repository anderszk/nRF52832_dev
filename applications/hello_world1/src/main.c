/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr.h>
#include <logging/log.h>
#include <dk_buttons_and_leds.h>
#include "remote.h"
#include "motor_control.h"

LOG_MODULE_REGISTER(LOG_MODULE_NAME);

static struct bt_conn *current_conn;    // Used to keep track of current connection status.

void on_connected(struct bt_conn *conn, uint8_t err);
void on_disconnected(struct bt_conn *conn, uint8_t reason);
void on_notif_changed(enum bt_button_notifications_enabled status);
void on_data_received(struct bt_conn *conn, const uint8_t *const data, uint16_t len);

struct bt_conn_cb bluetooth_callbacks = {
	.connected 	= on_connected,
	.disconnected 	= on_disconnected,
};

struct bt_remote_service_cb remote_callbacks = {
    .notif_changed = on_notif_changed,
    .data_received = on_data_received,
};

//LEDS
#define LOG_MODULE_NAME app
#define RUN_STATUS_LED DK_LED1
#define CONN_STATUS_LED DK_LED2
#define RUN_LED_BLINK_INTERVAL 1000

void on_data_received(struct bt_conn *conn, const uint8_t *const data, uint16_t len)
{
    uint8_t temp_str[len+1];
    memcpy(temp_str, data, len);
    temp_str[len] = 0x00;

    LOG_INF("Received data on conn %p. Len: %d", (void *)conn, len);
    LOG_INF("Data: %s", log_strdup(temp_str));
	if(data[0] == 0x00){
		set_motor_angle(1);
	}
	else if(data[0] == 0x01){
		set_motor_angle(2);
	}
}

void on_connected(struct bt_conn *conn, uint8_t err)
{
    if (err) {
        LOG_ERR("connection failed, err %d", err);
    }
	LOG_INF("Connected to central");
    current_conn = bt_conn_ref(conn);
    dk_set_led_on(CONN_STATUS_LED);
}

void on_disconnected(struct bt_conn *conn, uint8_t reason)
{
	LOG_INF("Disconnected (reason: %d)", reason);
	dk_set_led_off(CONN_STATUS_LED);
	if(current_conn) {
		bt_conn_unref(current_conn);
		current_conn = NULL;
	}
}

void button_handler(uint32_t button_state, uint32_t has_changed){
	int button_pressed = 0;
	if (has_changed & button_state){
		switch (has_changed){
			
			case DK_BTN1_MSK:
				button_pressed = 1;
				break;
			case DK_BTN2_MSK:
				button_pressed = 2;
				set_motor_angle(1);
				break;
			case DK_BTN3_MSK:
				button_pressed = 3;
				set_motor_angle(2);
				break;
			case DK_BTN4_MSK:
				button_pressed = 4;
				break;
			default:
				break;
		}
		set_button_value(button_pressed);
		LOG_INF("Button %d pressed", button_pressed);
		int err = send_button_notification(current_conn, button_pressed);
		if (err){
			LOG_ERR("Could not send button notification, err: %d", err);
		}
	}
}

static void configure_dk_buttons_leds(void){
	int err = dk_leds_init();
	if(err){
		LOG_ERR("Could not init LEDS(err %d",err);
	}

	err = dk_buttons_init(button_handler);
	if(err){
		LOG_ERR("Could not init BUTTONS(err %d", err);
	}
}

void on_notif_changed(enum bt_button_notifications_enabled status)
{
    if (status == BT_BUTTON_NOTIFICATIONS_ENABLED) {
        LOG_INF("Notifications enabled");
    } else {
        LOG_INF("Notifications disabled");
    }
}



void main(void)
{

	configure_dk_buttons_leds();
	int err = motor_init();
	if (err) {
    	LOG_ERR("motor_init() failed. (err %d)", err);
    }
	LOG_INF("Hello WorldS! %s\n", CONFIG_BOARD);

	err = bluetooth_init(&bluetooth_callbacks, &remote_callbacks);
	LOG_INF("running");

	for(;;){
		dk_set_led_on(RUN_STATUS_LED);
		k_sleep(K_MSEC(RUN_LED_BLINK_INTERVAL));
		dk_set_led_off(RUN_STATUS_LED);
		k_sleep(K_MSEC(RUN_LED_BLINK_INTERVAL));
	}

}
