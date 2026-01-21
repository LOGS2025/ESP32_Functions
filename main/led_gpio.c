#include "led_gpio.h"


void esp_Delay(int time){
    vTaskDelay(pdMS_TO_TICKS(time));
}

// Returns the value read for input
int LED_receive(int gpio_num_pin){ 
    return gpio_get_level(gpio_num_pin);
}

// Sets a pìn as output and on level 1
int LED_config_output(int gpio_pin_num){
    if(!GPIO_IS_VALID_OUTPUT_GPIO(gpio_pin_num)) return 1;

    // Reset any mode it had
    gpio_reset_pin(gpio_pin_num);

    // Set the mode    
    gpio_set_direction(gpio_pin_num, GPIO_MODE_OUTPUT);

    // Set output 0|1
    gpio_set_level(gpio_pin_num, 1);

    return 0;
}

// Input for the pin given on pullup mode
int LED_config_input(int gpio_pin_num){
    if(!GPIO_IS_VALID_OUTPUT_GPIO(gpio_pin_num)) return 1;

    // Reset any mode it had
    gpio_reset_pin(gpio_pin_num);

    // Set the mode    
    gpio_set_direction(gpio_pin_num, GPIO_MODE_INPUT);

    gpio_input_enable(gpio_pin_num);

    gpio_pullup_en(gpio_pin_num); // Agrega la resistencia adicional para adquirir la configuración pullup
    gpio_pulldown_dis(gpio_pin_num); /// Desactiva la otra configuración

    gpio_intr_disable(gpio_pin_num);

    return 0;
}

// Sets a blink pattern for 10 rounds and then resets the pin
void LED_blink(int gpio_num_pin){
    int i = 0;
    while(i < 10){
        gpio_set_level(gpio_num_pin, 0);
        vTaskDelay(pdMS_TO_TICKS(1000));

        gpio_set_level(gpio_num_pin, 1);
        esp_Delay(1000);

        i++;
        printf("Vuelta %i\n",i);
    }
    // Reset any mode it had
    gpio_reset_pin(gpio_num_pin);
}

