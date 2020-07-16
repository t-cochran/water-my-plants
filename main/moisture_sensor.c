#include "driver/adc.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "FreeRTOS.h"
#include "task.h"
#include "freertos/FreeRTOS.h"
#include "freertos/portmacro.h"

/*
 *  Read analog output from the moisture sensor on pin A0.
 *  
 *  Notes for later:
 * 
 *  xTaskCreate() : Create a new task, add to list of tasks run.
 *  vTaskDelete() : Remove a task from the RTOS kernel management.
 * 
 *  vTaskDelayUntil() : control period of task execution
 *  vTaskDelay() : delay a task for a number of ticks 
 */
void moisture_sensor_read()
{
    /* Configure the ADC pin to be read */
    adc_config_t cfg;
    cfg.mode = ADC_READ_TOUT_MODE;  // Read the adc pin
    cfg.clk_div = 8;                // Read time: clock=80M/clk_div [8, 32]

    /* Initialize the ADC pin*/
    adc_init(&cfg);

    /* Read the moisture sensor once every 1.5 seconds */
    const TickType_t xDelay = 1500 / portTICK_PERIOD_MS;

    /* Take some readings */
    unsigned short int pct = 0;
    while(1)
    {   
        /* Read from the ADC */
        uint16_t data = 0;
        adc_read(&data);

        /* Moisture percentage */
        pct = (845 - data) % 100; 

        /* Print the ADC value */
        ESP_LOGI("[adc check]", "MOISTURE LEVEL: %hu %%", (unsigned short int)pct);
        vTaskDelay(xDelay);
    }
}