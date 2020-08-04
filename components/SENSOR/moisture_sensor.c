#include "moisture_sensor.h"


/*
 *  Read analog output from the moisture sensor and compute the average.
 */
void moisture_sensor(void* pvParameter)
{
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
            /* Compute the moisture level */
            pct = 100 - adc_map(moisture_data, 370, 870, 0, 100); 
            ESP_LOGI("[adc check]", "MOISTURE LEVEL: %hu %%", (short int)pct);

            /* Sum moisture levels */
            if (pct > 0 && pct < 100)
            {
                total += pct;
                num_readings += 1;
            }
        }
        else
        {
            printf("Error reading from ADC: %d\n", err);
        }
        vTaskDelay(xDelay);
    }
    ESP_LOGI("[Average Moisture Level]", " %ld %%", (long)total/num_readings);
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