/* 
 *  test_project.c
 *  
 *  A starter project to flash and run on the ESP8266
 */
#include <stdio.h>
#include <unistd.h>
#include <esp_system.h>

void app_main(void)
{
    /* Obligatory */
    printf("Hello, World!\n");

    /* Simple loop before restarting */
    for (int i = 5; i >= 0; i--) {
        printf("Restarting in %d...\n", i);
        sleep(1);
    }
    printf("Restarting!\n");
    fflush(stdout);

    esp_restart();
}
