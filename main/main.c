/* 
 *  Main Task
 *      (1) Turn the water pump on/off.
 *  
 *  Fork two tasks:
 *      (1) Connect the ESP8266 to a WiFi access point.
 * 
 *      (2) Read analog input from a moisture sensor.
 */

/* Hardware modules */
#include "moisture_sensor.h"
#include "relay_module.h"


void app_main(void)
{    
    long moisture = 0;
    xQueue = NULL;

    /* Main task: Initialize nv flash memory partition */
    esp_err_t ret;
    if ((ret = nvs_flash_init()) == ESP_ERR_NVS_NO_FREE_PAGES) {
        nvs_flash_erase();
        ret = nvs_flash_init();
    }

    /* Main task: Initialize shared buffer */
    xQueue = xQueueCreate(10, sizeof(long));

    /* Task A: Initialize WiFi */
    xTaskCreate(&init_wifi, "wifi", 2048, NULL, 5, &wifi);
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    /* Main task: Pump water if moisture level is below 70% */
    while(1)
    {
        /* Task B: Read the moisture sensor */
        get_moisture_level();
        xQueueReceive(xQueue, (void*)&moisture, (TickType_t)5);

        /* Main task: Turn pump on for 10 seconds */
        if (moisture < 70)
        {
            water_pump_on(10);
        }
        vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
}