#ifndef __MOISTURE_SENSOR_H__
#define __MOISTURE_SENSOR_H__

/* 
 *  Libraries
 */
#include "driver/adc.h"
#include "FreeRTOS.h"
#include "wifi_connect.h"
#include "helpers.h"

/*
 *  Globals
 */
TaskHandle_t xMoisture;
QueueHandle_t xQueue;

/* 
 *  Function prototypes
 */
void moisture_sensor(void* pvParameter);
uint16_t adc_map(long x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max);
void get_moisture_level(void);

#endif // __MOISTURE_SENSOR_H__