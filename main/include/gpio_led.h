#ifndef __GPIO_LED_H__
#define __GPIO_LED_H__

#include "driver/gpio.h"  // gpio_config_t
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "portmacro.h"
#include "esp_err.h"

/* Select GPIO output pins */
#define GPIO12_OUTPUT       12  // D6 on PCB
#define GPIO15_OUTPUT       15  // D8 on PCB
#define GPIO_PINS_SELECTED  ((1ULL << GPIO12_OUTPUT) | (1ULL << GPIO15_OUTPUT))

/* GPIO function prototypes */
void GPIO12_blink(void *arg);
void GPIO15_solid(void *arg);
extern void gpio_config_init(gpio_config_t* cfg);

#endif  // __GPIO_LED_H__