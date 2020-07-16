#ifndef __WIFI_CONNECT_H__
#define __WIFI_CONNECT_H__

#include <stdbool.h>
#include <string.h>
#include "esp_log.h"
#include "esp_wifi.h"               // wifi_config_t
#include "driver/hw_timer.h"        // hw_timer_init
#include "freertos/event_groups.h"  // EventGroupHandle_t

/* WiFi Access Point configuration */
#define SSID        CONFIG_ESP_WIFI_SSID
#define PASSPHRASE  CONFIG_ESP_WIFI_PASSWORD
#define MAX_RETRY   CONFIG_ESP_MAXIMUM_RETRY

/* Create WiFi event group, event bits, retry count */
EventGroupHandle_t wifi_event_group;
#define WIFI_CONNECT        BIT0
#define WIFI_DISCONNECT     BIT1

extern void wifi_event_handler(void* arg, esp_event_base_t event, 
                               int32_t event_id, void* event_data);
extern void init_wifi(void);

#endif // __WIFI_CONNECT_H__