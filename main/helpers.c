#include "helpers.h"

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
            printf("\t%s Task ready\n", task_name);
            printf("*--------------------------------------------*\n");
            break;
        case eDeleted:
            printf("*--------------------------------------------*\n");
            printf("\t%s Task deleted\n", task_name);
            printf("*--------------------------------------------*\n");
            break;
        case eSuspended:
            printf("*--------------------------------------------*\n");
            printf("\t%s Task suspended\n", task_name);
            printf("*--------------------------------------------*\n");
            break;
        case eBlocked:
            printf("*--------------------------------------------*\n");
            printf("\t%s Task blocked\n", task_name);
            printf("*--------------------------------------------*\n");
            break;
        default:
            printf("*--------------------------------------------*\n");
            printf("\t%s Task unknown: %d\n", task_name, (int)task_info.eCurrentState);
            printf("*--------------------------------------------*\n");
            break;
    }
}