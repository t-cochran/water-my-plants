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
    gpio_config(cfg);
}

void init_LEDS(ledc_channel_config_t* ledc_channel, int pins[])
{
    /* Configure LED controller timer */
    ledc_timer_config_t ledc_timer = {
        .duty_resolution = LEDC_TIMER_13_BIT,
        .freq_hz = 5000,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .timer_num = LEDC_TIMER_0,
    };
    ledc_timer_config(&ledc_timer);

    /* Configure LED controller channels */
    for (int i=0; i < LED_COUNT; i++)
    {
        (ledc_channel + i) -> channel = i;
        (ledc_channel + i) -> duty = 0;
        (ledc_channel + i) -> gpio_num = pins[i];
        (ledc_channel + i) -> speed_mode = LEDC_LOW_SPEED_MODE;
        (ledc_channel + i) -> hpoint = 0;
        (ledc_channel + i) -> timer_sel = LEDC_TIMER_0;
        ledc_channel_config(ledc_channel + i);
    }
    ledc_fade_func_install(0);
}

void toggle_LED(ledc_channel_config_t ledc_channel, char* setting)
{
    if (strcmp(setting, "on") == 0)
    {
        ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, 128);
        ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
    }
    else if (strcmp(setting, "off") == 0)
    {
        ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, 0);
        ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
    }
    else
    {
        printf("Error toggle_LED setting: [on, off]\n");
        exit(1);
    }
}