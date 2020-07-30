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


/* 
 *  Function prototypes
 */
void moisture_sensor(void* pvParameter);


#endif // __MOISTURE_SENSOR_H__