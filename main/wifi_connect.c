#include "gpio_led.h"
#include "wifi_connect.h"

/* Track LED state during WiFi connect/disconnect */
bool LEDblink = false;
bool LEDsolid = false;

/*
 *  An event handler that responds to changes in WiFi state.
 */ 
void wifi_event_handler(void* arg, esp_event_base_t event, 
                        int32_t event_id, void* event_data)
{
    retry_count = 0;            // Current number of re-connect attempts
    const char* TAG = "WiFi";   // Log tag for the Wifi handler

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
        if (LEDblink)
        {
            hw_timer_deinit();
            LEDblink = false;
        }
        hw_timer_init(GPIO15_solid, NULL);
        hw_timer_alarm_us(1000, true);
        LEDsolid = true;
    }

    /* ESP8266 given IP address */
    else if (event == WIFI_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        /* Get the IP address of the ESP8266 and print it */
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        char* ip_str = ip4addr_ntoa(&event -> ip_info.ip);
        ESP_LOGI(TAG, "Device IP: %s\n", ip_str);

        /* Set wifi event group bits and reset retry count */
        xEventGroupSetBits(wifi_event_group, WIFI_CONNECT);   
        retry_count = 0;
    }

    /* ESP8266 disconnected */
    else if (event == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        /* LED indicates the connection was lost */
        ESP_LOGI(TAG, "Disconnected from SSID: %s\n", SSID);
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

        /* Re-connect to the access point */
        if (retry_count < MAX_RETRY)
        {
            esp_wifi_connect();
            ESP_LOGI(TAG, "Connecting...\n");
            retry_count++;
        }
        else
        {
            xEventGroupSetBits(wifi_event_group, WIFI_DISCONNECT);
        }
    }
}

/*
 *  Initialize the ESP8266 so it can create a WiFi connection.
 */
void init_wifi(void* pvParameter)
{
    /* Configure GPIO pins for LEDs indicating WiFi connection */
    gpio_config_t gpio_cfg;
    gpio_config_init(&gpio_cfg);

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
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_cfg);
    esp_wifi_start();

    /* Block until either a connection event or disconnect event */
    xEventGroupWaitBits(wifi_event_group,
           WIFI_CONNECT | WIFI_DISCONNECT,
           pdFALSE,
           pdFALSE,
           portMAX_DELAY);

    /* Unregister the wifi event handler from the system event loop */
    esp_event_handler_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler);
    esp_event_handler_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler);
    vEventGroupDelete(wifi_event_group);
}