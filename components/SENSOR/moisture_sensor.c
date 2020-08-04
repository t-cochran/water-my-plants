#include "moisture_sensor.h"


/*  
 *  Read the moisture sensor ADC pin
 * 
 *  Note: ESP8266 nodeMCU has a single ADC pin shared with the wifi module.
 *        Wifi is temporarily disconected during moisture sensor readings.
 */
void get_moisture_level(void)
{
    /* Task: Fork a task to read the moisture sensor */    
    if ((xRet = xTaskCreate(&moisture_sensor, "moisture", 2048, NULL, 5, &xMoisture)) == pdPASS)
    {
        /* Main task: Wait until the moisture sensor task completes */
        esp_wifi_disconnect();
        while (1)
        {
            vTaskGetInfo(xMoisture, &task_info, pdTRUE, eInvalid);
            if ((int) task_info.eCurrentState == eDeleted) { break; }
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
        task_status(xMoisture, "moisture sensor");
        esp_wifi_connect();
    }
}


/*
 *  Read analog output from the moisture sensor and compute the average.
 */
void moisture_sensor(void* pvParameter)
{
    long xMessage = 0;
    esp_err_t err;
    moisture_data = 0;
    long pct = 0, total = 0, num_readings = 0;

    /* Configure the ADC pin to be read */
    adc_config_t cfg;
    cfg.mode = ADC_READ_TOUT_MODE;  // Read the adc pin
    cfg.clk_div = 8;                // Read time: clock=80M/clk_div [8, 32]
    adc_init(&cfg);

    /* Read the moisture sensor once every second */
    const TickType_t xDelay = 1000 / portTICK_PERIOD_MS;

    /* Take 5 readings from the moisture sensor */
    for (int i=0; i < 5; i++)
    {   
        /* Read from the ADC */
        if ((err = adc_read(&moisture_data)) == ESP_OK)
        {
            /* Compute the percent moisture level */
            pct = 100 - adc_map(moisture_data, 370, 870, 0, 100); 
            if (pct > 0 && pct < 100)
            {
                total += pct;
                num_readings += 1;
            }
        }
        ESP_LOGI("[adc check]", "MOISTURE LEVEL: %hu %%", (short int)pct);
        vTaskDelay(xDelay);
    }
    ESP_LOGI("[Average Moisture Level]", " %ld %%", (long)total/num_readings);

    /* Send avg moisture level to the shared queue */
    xMessage = (long)total/num_readings;
    xQueueSend(xQueue, (void*)&xMessage, (TickType_t)5);

    /* De-init the adc pin and delete the task */
    adc_deinit();
    vTaskDelete(xMoisture);
}


/*
 *  Map analog input to a range of values
 */
long adc_map(long x, long in_min, long in_max, long out_min, long out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}