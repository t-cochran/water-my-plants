#ifndef __WIFI_CONNECT_H__
#define __WIFI_CONNECT_H__

/*
 *  Libraries
 */
#include "esp_log.h"
#include "gpio_led.h"
#include "esp_wifi.h"
#include "freertos/event_groups.h"

/*
 *  Constants
 */
#define SSID                CONFIG_ESP_WIFI_SSID
#define PASSPHRASE          CONFIG_ESP_WIFI_PASSWORD
#define WIFI_CONNECT        BIT0
#define WIFI_DISCONNECT     BIT1

/* 
 *  Globals
 */
EventGroupHandle_t wifi_event_group;
TaskHandle_t wifi;

/* 
 *  Function prototypes
 */
void wifi_event_handler(void* arg, esp_event_base_t event, 
                        int32_t event_id, void* event_data);
void init_wifi(void* pvParameter);
void wifi_disconnect(void);

#endif // __WIFI_CONNECT_H__