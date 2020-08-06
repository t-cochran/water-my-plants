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
    long xRecv = 0;
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

    /* Task B: Read the moisture sensor */
    get_moisture_level();
    xQueueReceive(xQueue, (void*)&xRecv, (TickType_t)5);
    printf("The average moisture level is: %ld\n", xRecv);
    sleep(2);

    /* Task B: Read the moisture sensor */
    get_moisture_level();
    xQueueReceive(xQueue, (void*)&xRecv, (TickType_t)5);
    printf("The average moisture level is: %ld\n", xRecv);
    sleep(2);

    /* Turn off Wifi */
    printf("turning wifi off\n");
    esp_wifi_disconnect();

    /* Task C: Turn on the water pump */
    /*
     *   TODO: Create function to:
     *          -> Fork Task C
     *          -> Turn on water pump for 10 seconds
     *          -> Delete Task C
     * 
     *   TODO: If the moisture level is below a threshold, fork Task C
     * 
     *   TODO: Test -- Empty cup; while(1) read moisture level every 10 seconds
     *                              if moisture level is below threshold
     *                                  fork task C to turn on pump for 10 seconds
     */
    // /* Main task: Turn the water pump on */
    // water_pump_on(10);
    // sleep(5);


    // /* Check moisture sensor and wifi task status */
    // task_status(xMoisture, "moisture_sensor");
    // task_status(wifi, "wifi");
    // sleep(5);
}