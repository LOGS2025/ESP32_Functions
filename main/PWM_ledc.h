#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"

#include "driver/ledc.h" // LED Control
#include "led_gpio.h"
#include "headers.h"

void PWM_ledc_init_timer(void);

void PWM_ledc_init_channel(int pot_gpio);

void PWM_change_Duty(int new_duty);

void PWM_set_targetDuty_safe(int duty, int scale, int cycle_num);

void PWM_set_0_Duty_safe(int scale, int cycle_num);

int PWM_ledc_get_duty(void);

int PWM_ledc_get_freq(void);

/*
            EXAMPLE SET FOR PWM OUTPUT
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
*/

/*
        TO INIT PWM TIMER AND CHANNEL CONFIGURATION DO
     Set the LEDC peripheral configuration
    PWM_ledc_init_timer();
    PWM_ledc_init_channel(POT_GPIO);
     Set duty to 50%
        ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, 0));
        // Update duty to apply the new value
        ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL));
*/