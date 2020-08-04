#ifndef __MOISTURE_SENSOR_H__
#define __MOISTURE_SENSOR_H__

/* 
 *  Libraries
 */
#include "driver/adc.h"
#include "esp_log.h"
#include "FreeRTOS.h"
#include "task.h"
#include "freertos/FreeRTOS.h"

/*
 *  Globals
 */
uint16_t moisture_data;
TaskHandle_t xMoisture;

/* 
 *  Function prototypes
 */
void moisture_sensor(void* pvParameter);
long adc_map(long x, long in_min, long in_max, long out_min, long out_max);

#endif // __MOISTURE_SENSOR_H__