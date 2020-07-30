/* 
 *  Main Task
 *      (1) Turn the water pump on/off.
 *  
 *  Fork two RTOS tasks:
 *      (1) Connect the ESP8266 to a WiFi access point.
 *          Display WiFi connection status using GPIO12 and GPIO1 pins.
 * 
 *      (2) Read analog input from a moisture sensor and display the data in the log.
 */

/* Hardware modules */
#include "gpio_led.h"
#include "wifi_connect.h"
#include "moisture_sensor.h"
#include "relay_module.h"

/* Main headers */
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>

void app_main(void)
{    
    /* Init nv flash memory partition */
    esp_err_t ret;
    if ((ret = nvs_flash_init()) == ESP_ERR_NVS_NO_FREE_PAGES) {
        nvs_flash_erase();
        ret = nvs_flash_init();
    }

    /* Task: Create WiFi connection */
    TaskHandle_t xWifi;
    xTaskCreate(&init_wifi, "wifi", 2048, NULL, 5, &xWifi);

    /* Task: Read the moisture sensor 
     *
     * TODO: Pass TaskHandle_t xWifi to xMoisture task
     *       Turn off WiFi. 
     *       Read moisture.
     *       Turn WiFi back on.
     * */
    TaskHandle_t xMoisture;
    xTaskCreate(&moisture_sensor, "moisture", 2048, NULL, 5, &xMoisture);

    /* Main Task: Turn the water pump on a few times */
    water_pump_on(10);
    sleep(5);
    water_pump_on(10);
    sleep(5);
    water_pump_on(10);
    sleep(5);
}