#include "relay_module.h"

/* 
 *  Turn current on in relay module for a given duration
 */
void water_pump_on(unsigned int seconds)
{
    /* Set GPIO pin settings for pin 4 */
    gpio_config_t cfg;
    cfg.intr_type = GPIO_INTR_DISABLE;       // disable interrupt
    cfg.mode = GPIO_MODE_OUTPUT;             // output mode
    cfg.pin_bit_mask = RELAY_MODULE_PINS;    // bit mask of selected pins
    cfg.pull_down_en = 0;                    // disable pull down
    cfg.pull_up_en = 0;                      // disable pull up
    gpio_config(&cfg);

    /* Current flows for duration (seconds) */
    gpio_set_level(4, ON);
    sleep(seconds);
    gpio_set_level(4, OFF);
}