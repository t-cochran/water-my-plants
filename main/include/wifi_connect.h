#ifndef __WIFI_CONNECT_H__
#define __WIFI_CONNECT_H__

#include "freertos/event_groups.h"  // EventGroupHandle_t

/* WiFi Access Point configuration */
#define SSID        CONFIG_ESP_WIFI_SSID
#define PASSPHRASE  CONFIG_ESP_WIFI_PASSWORD
#define MAX_RETRY   CONFIG_ESP_MAXIMUM_RETRY

/* Create WiFi event group, event bits, retry count */
static EventGroupHandle_t wifi_event_group;
static int retry_count = 0;
#define WIFI_CONNECT        BIT0
#define WIFI_DISCONNECT     BIT1

#endif // __WIFI_CONNECT_H__