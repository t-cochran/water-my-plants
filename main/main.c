/* 
 *  Main Task
 *      (1) Turn the water pump on/off.
 *  
 *  Fork two RTOS tasks:
 *      (1) Connect the ESP8266 to a WiFi access point.
 *          Display WiFi connection status using GPIO12 and GPIO15 pins.
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

#define INCLUDE_vTaskDelete 1

void app_main(void)
{
    /* Init the nv flash memory partition on the ESP8266 */
    esp_err_t ret;
    if ((ret = nvs_flash_init()) == ESP_ERR_NVS_NO_FREE_PAGES) {
        nvs_flash_erase();
        ret = nvs_flash_init();
    }

    /* Task: Create WiFi connection */
    TaskHandle_t xWifi;
    xTaskCreate(&init_wifi, "wifi", 2048, NULL, 5, &xWifi);

    /* Task: Read the moisture sensor */
    TaskHandle_t xMoisture;
    xTaskCreate(&moisture_sensor, "moisture", 2048, NULL, 5, &xMoisture);

    /* Main Task: Turn the water pump on a few times */
    printf("TURNING ON WATER PUMP\n");
    water_pump_on(10);
}