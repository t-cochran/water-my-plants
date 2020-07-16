set(COMPONENT_SRCS "main.c"
                   "gpio_led.c"
                   "wifi_connect.c"
                   "moisture_sensor.c")
set(COMPONENT_ADD_INCLUDEDIRS ". include")

register_component()