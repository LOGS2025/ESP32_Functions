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
//    ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, 0));
//    // Update duty to apply the new value
//    ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL));

    int duty = 1000;
    int cycle_num = 10 ;
    int scale = 2 ;

    // Init fade thread
    ledc_fade_func_install(0); // 0 is default interrupt allocation
    
    int n = 20, i = 0;
    // Loop n times
    while(i < n)
    {
        PWM_set_targetDuty_safe(duty, scale, cycle_num);

        printf("ledc_get_duty(): %d\n", PWM_ledc_get_duty());
        printf("ledc_get_freq(): %d\n", PWM_ledc_get_freq());

        esp_Delay(1000);    
        
        PWM_set_0_Duty_safe(scale, cycle_num);
        esp_Delay(1000);    


        i++;
    }

    ledc_stop(LEDC_MODE, LEDC_CHANNEL,0);
    ledc_fade_func_uninstall();
}
