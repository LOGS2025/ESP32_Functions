#include "PWM_ledc.h"

void PWM_change_Duty(int new_duty){
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, new_duty));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL));
}

void PWM_ledc_init_timer(void){
        //PWM 
    // LEDC PWM timer config
    ledc_timer_config_t ledc_timer = {
        .freq_hz = LEDC_FREQUENCY,
        .speed_mode = LEDC_MODE,
        .timer_num = LEDC_TIMER,
        .duty_resolution = LEDC_DUTY_RES,
        .clk_cfg = LEDC_AUTO_CLK
    };

    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));
}

void PWM_ledc_init_channel(int pot_gpio){

    // LEDC PWM channel config
    ledc_channel_config_t ledc_channel = {
        .speed_mode     = LEDC_MODE,
        .channel        = LEDC_CHANNEL,
        .timer_sel      = LEDC_TIMER,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = pot_gpio,
        .duty           = 3277, // Set duty to 0%
        .hpoint         = 0
    };

    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
}

int PWM_ledc_get_duty(void){
    return ledc_get_duty(LEDC_MODE, LEDC_CHANNEL);
}
int PWM_ledc_get_freq(void){
    return ledc_get_freq(LEDC_MODE, LEDC_TIMER);
}

void PWM_set_targetDuty_safe(int duty, int scale, int cycle_num){
    ledc_set_fade_step_and_start(
        LEDC_MODE, 
        LEDC_CHANNEL, 
        duty, 
        scale, 
        cycle_num,
        LEDC_FADE_NO_WAIT    // LEDC fade function will return immediately
    );
    esp_Delay(1000);
}

void PWM_set_0_Duty_safe(int scale, int cycle_num){
    ledc_set_fade_step_and_start(
        LEDC_MODE, 
        LEDC_CHANNEL, 
        0, 
        scale, 
        cycle_num,
        LEDC_FADE_NO_WAIT
    );
    esp_Delay(1000);
}