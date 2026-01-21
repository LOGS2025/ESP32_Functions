idf_component_register(SRCS "PWM_analog_read.c" "PWM_ledc.c" "led_gpio.c" "main.c"
                       PRIV_REQUIRES spi_flash
                       REQUIRES esp_driver_gpio
                       REQUIRES esp_driver_ledc
                       REQUIRES esp_adc
                       INCLUDE_DIRS "")
