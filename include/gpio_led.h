#ifndef __GPIO_LED_H__
#define __GPIO_LED_H__

/*
 *  Libraries
 */
#include <string.h>
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"

/*
 *  Constants
 */
#define LED_COUNT  2

/*
 *  Function protoypes
 */
void init_LEDS(ledc_channel_config_t* ledc_channel, const int pins[]);
void toggle_LED(ledc_channel_config_t ledc_channel, char* setting);

#endif  // __GPIO_LED_H__