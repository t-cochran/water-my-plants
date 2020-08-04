#include "helpers.h"


/*  
 *  Read the moisture sensor ADC pin
 * 
 *  Note: ESP8266 nodeMCU has a single ADC pin shared with the wifi module.
 *        Wifi is temporarily disconected during moisture sensor readings.
 */
void get_moisture_level(void)
{
    /* Task B: Fork a task to read the moisture sensor */    
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
 *  Print the status of a task
 */
void task_status(TaskHandle_t task_handle, char* task_name)
{
    vTaskGetInfo(task_handle, &task_info, pdTRUE, eInvalid);
    switch((int)task_info.eCurrentState)
    {
        case eReady:
            printf("*--------------------------------------------*\n");
            printf("\t\t%s Task ready\n", task_name);
            printf("*--------------------------------------------*\n");
            break;
        case eDeleted:
            printf("*--------------------------------------------*\n");
            printf("\t\t%s Task deleted\n", task_name);
            printf("*--------------------------------------------*\n");
            break;
        case eSuspended:
            printf("*--------------------------------------------*\n");
            printf("\t\t%s Task suspended\n", task_name);
            printf("*--------------------------------------------*\n");
            break;
        case eBlocked:
            printf("*--------------------------------------------*\n");
            printf("\t\t%s Task blocked\n", task_name);
            printf("*--------------------------------------------*\n");
            break;
        default:
            printf("*--------------------------------------------*\n");
            printf("\t\t%s Task unknown: %d\n", task_name, (int)task_info.eCurrentState);
            printf("*--------------------------------------------*\n");
            break;
    }
}