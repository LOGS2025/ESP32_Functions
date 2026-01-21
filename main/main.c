#include "led_gpio.h"
#include "PWM_ledc.h"

#include "esp_err.h"

#define POT_GPIO          (5) // Define the output GPIO


void app_main()
{
    // Set the LEDC peripheral configuration
    PWM_ledc_init_timer();
    PWM_ledc_init_channel(POT_GPIO);
    // Set duty to 50%
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, LEDC_DUTY));
    // Update duty to apply the new value
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL));

    return;
}
