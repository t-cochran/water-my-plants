#ifndef __HELPERS_H__
#define __HELPERS_H__

/*
 *  Libraries
 */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "moisture_sensor.h"
#include "wifi_connect.h"
#include <string.h>
#include <stdio.h>

/*
 *  Constants
 */
#define USE_TRACE_FACILITY  1
#define TaskGetState    1

/*
 *  Gloabls
 */
BaseType_t xRet;
TaskHandle_t wifi;
TaskStatus_t task_info;

/*
 *  Function protoypes
 */
void task_status(TaskHandle_t task_handle, char* task_name);
void get_moisture_level(void);

#endif  // __HELPERS_H__