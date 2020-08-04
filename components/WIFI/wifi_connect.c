#include "gpio_led.h"
#include "wifi_connect.h"
#include "esp_log.h"

/*
 *  Initialize LED pins and channels
 */
const int led_gpio_pins[LED_COUNT] = { 12, 13 };
ledc_channel_config_t ledc_channel[LED_COUNT];
ledc_channel_config_t GREEN_LED;
ledc_channel_config_t RED_LED;

/*
 *  Event handler that responds to changes in WiFi state
 */ 
void wifi_event_handler(void* arg, esp_event_base_t event, 
                        int32_t event_id, void* event_data)
{
    const char* TAG = "WiFi";   // Log tag for the Wifi handler

    printf("EVENT TYPE TO BE HANDLED: %d\n", event_id);

    /* ESP8266 has started connecting */
    if (event == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        esp_wifi_connect();
        ESP_LOGI(TAG, "Establishing connection to SSID: %s\n", SSID);
    }

    /* ESP8266 connected */
    else if (event == WIFI_EVENT && event_id == WIFI_EVENT_STA_CONNECTED)
    {
        /* LED indicates a connection was established */
        ESP_LOGI(TAG, "Connected to SSID: %s \n", SSID);    
        toggle_LED(GREEN_LED, "on");
        toggle_LED(RED_LED, "off");
            printf("-----------x-x---xx-x-x-x-x->>>>>>>>>>> 0\n");
    }

    /* ESP8266 given IP address */
    else if (event == WIFI_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        /* Get the IP address of the ESP8266 and print it */
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        char* ip_str = ip4addr_ntoa(&event -> ip_info.ip);
        ESP_LOGI(TAG, "Device IP: %s\n", ip_str);

        /* Set wifi event group bits */
        xEventGroupSetBits(wifi_event_group, WIFI_CONNECT);   
    }

    /* ESP8266 disconnected */
    else if (event == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        /* LED indicates the connection was lost */
        ESP_LOGI(TAG, "Disconnected from SSID: %s\n", SSID);
        toggle_LED(GREEN_LED, "off");
        toggle_LED(RED_LED, "on");
            printf("-----------x-x---xx-x-x-x-x->>>>>>>>>>> 1\n");
    }
}

/*
 *  Initialize WiFi connection
 */
void init_wifi(void* pvParameter)
{
    /* Configure GPIO pins for LEDs indicating WiFi connection */
    init_LEDS(ledc_channel, led_gpio_pins);
    GREEN_LED = ledc_channel[0];
    RED_LED = ledc_channel[1];

    /* Initialize the TCIP/IP stack */
    tcpip_adapter_init();

    /* Create an event group for WiFi events */
    wifi_event_group = xEventGroupCreate();
    esp_event_loop_create_default();

    /* Create a WiFi config structure and pass it to the driver */
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    /* Register the wifi event handler */
    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL);

    /* Set the access point configuration */
    wifi_config_t wifi_cfg = 
    {
        .sta = 
        { 
            .ssid = SSID,
            .password = PASSPHRASE,
        },
    };

    /* Limit WiFi connection to WPA2 access points */
    if (strlen((char *) wifi_cfg.sta.password)) {
        wifi_cfg.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
    }

    /* Set the WiFi operating mode as a station and start it */
        printf("-----------x-x---xx-x-x-x-x->>>>>>>>>>> 2\n");
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_cfg);
    esp_wifi_start();
    printf("-----------x-x---xx-x-x-x-x->>>>>>>>>>> 3\n");
    /* Block until a connection event or disconnect event */
    xEventGroupWaitBits(wifi_event_group,
           WIFI_CONNECT | WIFI_DISCONNECT,
           pdFALSE,
           pdFALSE,
           portMAX_DELAY);

    printf("-----------x-x---xx-x-x-x-x->>>>>>>>>>> 4\n");

    /* Unregister the wifi event handler from the system event loop */
    esp_event_handler_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler);
    esp_event_handler_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler);
    vEventGroupDelete(wifi_event_group);
        printf("-----------x-x---xx-x-x-x-x->>>>>>>>>>> 5\n");
}