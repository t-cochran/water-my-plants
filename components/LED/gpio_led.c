#include "gpio_led.h"

/* 
 *  Configure the LED PWM controller
 */
void init_LEDS(ledc_channel_config_t* ledc_channel, const int pins[])
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

/* 
 *  Turn LED channels on/off
 */
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