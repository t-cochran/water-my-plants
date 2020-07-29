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

#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "portmacro.h"
#include "esp_err.h"

#define GPIO_LED_PIN            4     // D2 on esp8266
#define GPIO_LED_DELAY          1000  // milliseconds
#define GPIO_LED_BRIGHTNESS     128
#define GPIO_LED_OFF            0

/* Configure the LED controller timer */
ledc_timer_config_t ledc_timer = {
    .duty_resolution = LEDC_TIMER_13_BIT,
    .freq_hz = 5000,
    .speed_mode = LEDC_HIGH_SPEED_MODE,
    .timer_num = LEDC_TIMER_0,
};

/* Configure the LED controller channel */
ledc_channel_config_t ledc_channel = {
    .channel    = LEDC_CHANNEL_0,
    .duty       = 0,
    .gpio_num   = GPIO_LED_PIN,
    .speed_mode = LEDC_LOW_SPEED_MODE,
    .hpoint     = 0,
    .timer_sel  = LEDC_TIMER_0
};

void app_main(void)
{
    /* Set config for timers used by LED controller */
    ledc_timer_config(&ledc_timer);

    /* Set config for LED controller channel */
    ledc_channel_config(&ledc_channel);

    /* Initailize fade service */
    ledc_fade_func_install(0);

    while(1)
    {
        printf("3. LEDC set duty = %d without fade\n", GPIO_LED_BRIGHTNESS);
        ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, GPIO_LED_BRIGHTNESS);
        ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
        vTaskDelay(GPIO_LED_DELAY / portTICK_PERIOD_MS);

        printf("4. LEDC set duty = 0 without fade\n");
        ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, GPIO_LED_OFF);
        ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
        vTaskDelay(GPIO_LED_DELAY / portTICK_PERIOD_MS);
    }

/*     ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, 4096);
    ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
    vTaskDelay(1000 / 1000); */

    /* Init the nv flash memory partition on the ESP8266 */
/*     esp_err_t ret;
    if ((ret = nvs_flash_init()) == ESP_ERR_NVS_NO_FREE_PAGES) {
        nvs_flash_erase();
        ret = nvs_flash_init();
    } */

/*     ledc_timer_config(&ledc_timer);
    ledc_channel_config(&ledc_channel);
    ledc_fade_func_install(0); */

    // //  while(1)
    // //  {
    //        ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, 500);
    //        printf("sleeping for 3...\n");
    //        sleep(3);
    //        ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
    //        for (int i=0; i < 5; i++) 
    //        {
    //            printf("sleeping for %d ...\n", i);
    //            sleep(1);
    //        }
    //        printf("gpio num before: %d\n", ledc_channel.gpio_num);
    //        ledc_channel.gpio_num = 12;
    //        ledc_channel.channel = LEDC_CHANNEL_1;
    //        printf("gpio num after: %d\n", ledc_channel.gpio_num);

    //        //ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, 0);
    //        ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
            //ledc_stop(ledc_channel.speed_mode, ledc_channel.channel, true);
            //sleep(3);
        
    //      vTaskDelay(500 / portTICK_PERIOD_MS);

    //         ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, 0);
    //         ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
    //         vTaskDelay(500 / portTICK_PERIOD_MS);
    //  }


    /* Task: Create WiFi connection */
    // TaskHandle_t xWifi;
    // xTaskCreate(&init_wifi, "wifi", 2048, NULL, 5, &xWifi);

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
