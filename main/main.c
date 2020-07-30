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

void app_main(void)
{
    /* Set LED gpio pins */
    int led_gpio_pins[LED_COUNT] = { 12, 13 };

    /* Initialize LEDs */
    ledc_channel_config_t ledc_channel[LED_COUNT];
    init_LEDS(ledc_channel, led_gpio_pins);

    /* Declare LED types */
    const ledc_channel_config_t green = ledc_channel[0];
    const ledc_channel_config_t red = ledc_channel[1];

    /* Toggle LEDs on/off */
    toggle_LED(green, "on");
    toggle_LED(red, "off");
    
    /* Task: Create WiFi connection */
/*     TaskHandle_t xWifi;
    xTaskCreate(&init_wifi, "wifi", 2048, NULL, 5, &xWifi);
 */
    // /* Task: Read the moisture sensor */
    // TaskHandle_t xMoisture;
    // xTaskCreate(&moisture_sensor, "moisture", 2048, NULL, 5, &xMoisture);

    /* Main Task: Turn the water pump on a few times */
    // printf("TURNING ON WATER PUMP\n");
    // water_pump_on(10);
    // sleep(5);
    // water_pump_on(10);
    // sleep(5);
    // water_pump_on(10);
    // sleep(5);
}