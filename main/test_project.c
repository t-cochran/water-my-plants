/* 
 *  test_project.c
 *  
 *  Connect the ESP8266 to a WiFi access point.
 *  Display WiFi connection status using GPIO12 and GPIO15 pins.
 */
#include <stdio.h>
#include "esp_wifi.h"               // wifi_config_t
#include "driver/gpio.h"            // gpio_config_t
#include "driver/hw_timer.h"        // hw_timer_init
#include "freertos/event_groups.h"  // EventGroupHandle_t
#include "esp_log.h"                // ESP_LOG_NONE
#include "nvs_flash.h"              // ESP_ERR_NVS_NO_FREE_PAGES

/* WiFi Access Point */
#define SSID        "-"
#define PASSPHRASE  "-"

/* Designate GPIO output pins */
#define GPIO12_OUTPUT       12  // D6 on PCB
#define GPIO15_OUTPUT       15  // D8 on PCB
#define GPIO_PINS_SELECTED  ((1ULL << GPIO12_OUTPUT) | (1ULL << GPIO15_OUTPUT))

/* Create a log tag and RTOS event group */
static const char* TAG = "main";
static EventGroupHandle_t wifi_event_group;

/* Save the state of the hardware timer */
bool LEDblink = false;
bool LEDsolid = false;

/* Alternate GPIO12 pin output */
void GPIO12_blink(void *arg)
{
    static int state = 0;
    gpio_set_level(GPIO15_OUTPUT, 0);
    gpio_set_level(GPIO12_OUTPUT, (state++) % 2);
}

/* Turn on GPIO15 pin output */
void GPIO15_solid(void *arg)
{
    gpio_set_level(GPIO12_OUTPUT, 0);
    gpio_set_level(GPIO15_OUTPUT, 1);
}

/* Set GPIO pin settings for pins 12 and 15 */
void gpio_config_init(gpio_config_t* cfg)
{
    ESP_LOGI(TAG, "Starting GPIO config...");
    cfg -> intr_type = GPIO_INTR_DISABLE;
    cfg -> mode = GPIO_MODE_OUTPUT;
    cfg -> pin_bit_mask = GPIO_PINS_SELECTED;
    cfg -> pull_down_en = 0;
    cfg -> pull_up_en = 0;
}

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
        ESP_LOGI(TAG, "Error (%d) Hostname could not be set.", ret);  
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
            ESP_LOGI(TAG, "Attempting to connect to: %s\n", SSID);
            break;

        /* ESP8266 is connected to an access point */
        case SYSTEM_EVENT_STA_CONNECTED:
            ESP_LOGI(TAG, "Connected to WiFi Access Point: \n");

            /* LED indicates a connection was established */
            if (LEDblink)
            {
                hw_timer_deinit();
                LEDblink = false;
            }
            hw_timer_init(GPIO15_solid, NULL);
            hw_timer_alarm_us(1000, true);
            LEDsolid = true;
            break;

        /* ESP8266 got an IP address from the access point */
        case SYSTEM_EVENT_STA_GOT_IP:
            xEventGroupSetBits(wifi_event_group, 1);   

            /* Get the IP address of the ESP8266 and print it */
            tcpip_adapter_ip_info_t ip_info;
            tcpip_adapter_get_ip_info(TCPIP_ADAPTER_IF_STA, &ip_info);
            break;

        /* ESP8266 is disconnected from the access point */
        case SYSTEM_EVENT_STA_DISCONNECTED:
            esp_wifi_connect();
            xEventGroupClearBits(wifi_event_group, 1);
            ESP_LOGI(TAG, "Disconnected  WiFi Access Point\n");

            /* LED indicates the connection was lost */
            if (LEDsolid || LEDblink)
            {
                hw_timer_disarm();
                hw_timer_deinit();
                LEDsolid = false;
            }
            hw_timer_init(GPIO12_blink, NULL);
            hw_timer_alarm_us(100000, true);
            vTaskDelay(3000 / portTICK_RATE_MS);  
            LEDblink = true;
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
    /* Configure GPIO pins */
    gpio_config_t io_conf;
    gpio_config_init(&io_conf);
    gpio_config(&io_conf);

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