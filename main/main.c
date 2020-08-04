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
#include "gpio_led.h"
#include "wifi_connect.h"
#include "moisture_sensor.h"
#include "relay_module.h"

/* Main headers and helper functions */
#include "helpers.h"

void app_main(void)
{    
    /* Init nv flash memory partition */
    esp_err_t ret;
    if ((ret = nvs_flash_init()) == ESP_ERR_NVS_NO_FREE_PAGES) {
        nvs_flash_erase();
        ret = nvs_flash_init();
    }

    /* Task A: Initialize WiFi */
    xTaskCreate(&init_wifi, "wifi", 2048, NULL, 5, &wifi);
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    /* Task B: Read the moisture sensor */
    get_moisture_level();
    /*
     *
     *   TODO: Return the average moisture level
     */
    printf("Sleeping for 5 seconds ...\n");
    get_moisture_level();

    /* Task C: Turn on the water pump */
    /*
     *
     *   TODO: Turn on the water pump if moisture level is below a threshold
     */

    // /* Main task: Turn the water pump on */
    // water_pump_on(10);
    // sleep(5);


    // /* Check moisture sensor and wifi task status */
    // task_status(xMoisture, "moisture_sensor");
    // task_status(wifi, "wifi");
    // sleep(5);
}