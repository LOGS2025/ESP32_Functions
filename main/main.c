#include "led_gpio.h"
#include "PWM_ledc.h"
#include "PWM_analog.read.h"


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
    //    ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, 0));
    //    // Update duty to apply the new value
    //    ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL));

    int duty = 1000;
    int cycle_num = 10 ;
    int scale = 2 ;


}

double frequency(){

}