#ifndef __RELAY_MODULE_H__
#define __RELAY_MODULE_H__

/*
 *  Libraries
 */
#include <unistd.h>
#include "driver/gpio.h"

/*
 *  Constants
 */
#define GPIO2_OUTPUT         4  // D2 on PCB
#define RELAY_MODULE_PINS    (1ULL << GPIO2_OUTPUT)
#define ON                   0
#define OFF                  1

/* 
 *  Function prototypes
 */
void water_pump_on(unsigned int seconds);

#endif // __RELAY_MODULE_H__