/* 
 *  Main Task
 *  
 *  Connect the ESP8266 to a WiFi access point.
 *  Display WiFi connection status using GPIO12 and GPIO15 pins.
 */
#include "nvs_flash.h"              // ESP_ERR_NVS_NO_FREE_PAGES
#include "gpio_led.h"
#include "wifi_connect.h"
#include "moisture_sensor.h"

/* 
 *  Main function
 */
void app_main(void)
{
    /* Init the nv flash memory partition on the ESP8266 */
    esp_err_t ret;
    if ((ret = nvs_flash_init()) == ESP_ERR_NVS_NO_FREE_PAGES) {
        nvs_flash_erase();
        ret = nvs_flash_init();
    }

    /* Configure GPIO pins for LEDs */
    gpio_config_t io_conf;
    gpio_config_init(&io_conf);
    gpio_config(&io_conf);


    /* Init WiFi connection */
    init_wifi();
    moisture_sensor_read();
}