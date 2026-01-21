#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"

#include "driver/gpio.h" // GPIO pin control

int LED_config_output(int gpio_pin_num);

int LED_config_input(int gpio_pin_num);

int LED_receive(int gpio_num_pin);

void LED_blink(int gpio_num_pin);
