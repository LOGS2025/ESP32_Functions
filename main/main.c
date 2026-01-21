#include "led_gpio.h"
#include "PWM_ledc.h"

#include "esp_err.h"

#define POT_GPIO GPIO_NUM_5 // Define the output GPIO
#define IO_GPIO GPIO_NUM_6

void change_Duty(int new_duty){
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, new_duty));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL));
}

void app_main()
{
    // Set the LEDC peripheral configuration
    PWM_ledc_init_timer();
    PWM_ledc_init_channel(POT_GPIO);
    // Set duty to 50%
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, LEDC_DUTY));
    // Update duty to apply the new value
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL));

    int duty = 4096;

    while(1){
        printf("ledc_get_duty(): %d\n", PWM_ledc_get_duty());
        printf("ledc_get_freq(): %d\n", PWM_ledc_get_freq());
        esp_Delay(250);
        if (duty > 0){
            duty = duty - 100;
            change_Duty(duty);
        } else {
            change_Duty(0);
            break;
        }
    }

    ledc_stop(LEDC_MODE, LEDC_CHANNEL,0);
    
    return;
}
