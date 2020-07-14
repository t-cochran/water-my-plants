/* 
 *  test_project.c
 *  
 *  Connect the ESP8266 to a WiFi access point.
 * 
 */
#include <stdio.h>
#include "esp_wifi.h"                  // wifi_config_t
#include "freertos/FreeRTOS.h"         // BIT0
#include "freertos/event_groups.h"     // EventGroupHandle_t
#include "esp_log.h"                   // ESP_LOG_NONE
#include "nvs_flash.h"                 // ESP_ERR_NVS_NO_FREE_PAGES

/* Constants */
#define SSID NULL
#define PASSPHRASE NULL
#define MAIN_TAG "main"
const int CONNECTED_BIT = 1;

/* Global handle to an RTOS event group */
static EventGroupHandle_t wifi_event_group;

/*
 *  Initialize the ESP8266 so it can create a WiFi connection.
 */
static void init_wifi(void)
{
    /* Initialize the TCIP/IP stack */
    tcpip_adapter_init();

    /* Create a WiFi config structure and pass it to the driver */
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    /* Set the WiFi operating mode as a station and start it */
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_start();

    /* Set the hostname */
    esp_err_t ret = tcpip_adapter_set_hostname(TCPIP_ADAPTER_IF_STA, "scarlett");
    if(ret != ESP_OK)
    {
        ESP_LOGI(MAIN_TAG, "Error (%d) Hostname could not be set.", ret);  
    }
}

/*
 *  Connect the ESP8266 to a given WiFi access point
 */ 
void wifi_connect(void)
{
    /* Set the access point configuration */
    wifi_config_t cfg = 
    {
        .sta = 
        { 
            .ssid = SSID,
            .password = PASSPHRASE,
        },
    };
    esp_wifi_disconnect();
    esp_wifi_set_config(ESP_IF_WIFI_STA, &cfg);
    esp_wifi_connect();
}

/*
 *  An event handler that responds to changes in WiFi state.
 */
static esp_err_t event_handler(void* ctx, system_event_t* event)
{
    switch(event -> event_id)
    {
        /* ESP8266 has started as a station */
        case SYSTEM_EVENT_STA_START:
            wifi_connect();
            ESP_LOGI(MAIN_TAG, "Connected to WiFi Access Point\n");
            break;
        
        /* ESP8266 got an IP address from the access point */
        case SYSTEM_EVENT_STA_GOT_IP:
            xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);   

            /* Get the IP address of the ESP8266 and print it */
            tcpip_adapter_ip_info_t ip_info;
	        tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA, &ip_info);
            print("Device IP Address: %s \n", ip4addr_ntoa(&ip_info.ip));
            break;

        /* ESP8266 is disconnected from the access point */
        case SYSTEM_EVENT_STA_DISCONNECTED:
            esp_wifi_connect();
            xEventGroupClearBits(wifi_event_group, CONNECTED_BIT);
            ESP_LOGI(MAIN_TAG, "Disconnected  WiFi Access Point\n");
            break;
        default:
            break;
    }
    return ESP_OK;
}

/* 
 *  Main function
 */
void app_main(void)
{
    /* Init an event loop to respond to events */
    esp_event_loop_init(event_handler, NULL);

    /* Create an event group to track events */
    wifi_event_group = xEventGroupCreate();

    /* Init the nv flash memory partition on the ESP8266 */
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES) {
        nvs_flash_erase();
        ret = nvs_flash_init();
    }
    /* Init the ESP8266 to make a connection with an access point */
    init_wifi();
}
