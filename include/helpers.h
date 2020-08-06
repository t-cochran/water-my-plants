#ifndef __HELPERS_H__
#define __HELPERS_H__

/*
 *  Libraries
 */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"

/*
 *  Constants
 */
#define USE_TRACE_FACILITY  1
#define TaskGetState        1

/* 
 *  Globals
 */
TaskStatus_t task_info;

/*
 *  Function protoypes
 */
void task_status(TaskHandle_t task_handle, char* task_name);

#endif  // __HELPERS_H__