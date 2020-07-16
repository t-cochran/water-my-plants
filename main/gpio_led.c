#include "gpio_led.h"

/* Alternate GPIO12 pin output */
void GPIO12_blink(void *arg)
{
    static int state = 0;
    gpio_set_level(GPIO15_OUTPUT, 0);
    gpio_set_level(GPIO12_OUTPUT, (state++) % 2);
}

/* Turn on GPIO15 pin output */
void GPIO15_solid(void *arg)
{
    gpio_set_level(GPIO12_OUTPUT, 0);
    gpio_set_level(GPIO15_OUTPUT, 1);
}

/* Set GPIO pin settings for pins 12 and 15 */
extern void gpio_config_init(gpio_config_t* cfg)
{
    cfg -> intr_type = GPIO_INTR_DISABLE;       // disable interrupt
    cfg -> mode = GPIO_MODE_OUTPUT;             // output mode
    cfg -> pin_bit_mask = GPIO_PINS_SELECTED;   // bit mask of selected pins
    cfg -> pull_down_en = 0;                    // disable pull down
    cfg -> pull_up_en = 0;                      // disable pull up
}