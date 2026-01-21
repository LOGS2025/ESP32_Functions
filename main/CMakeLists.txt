idf_component_register(SRCS "PWM_ledc.c" "led_gpio.c" "main.c"
                       PRIV_REQUIRES spi_flash
                       REQUIRES esp_driver_gpio
                       REQUIRES esp_driver_ledc
                       INCLUDE_DIRS "")
