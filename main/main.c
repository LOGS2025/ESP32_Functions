#include "led_gpio.h"
#include "PWM_ledc.h"
#include "PWM_analog.read.h"


#include "esp_err.h"

#define POT_GPIO GPIO_NUM_5 // Define the output GPIO
#define IO_GPIO 


//int duty = 1000;
//int cycle_num = 10 ;
//int scale = 2 ;

typedef struct ADC_Monitor_cfg_pkg {
    adc_oneshot_unit_handle_t handle_t;
    adc_oneshot_chan_cfg_t channel_config_t;
    adc_channel_t channel;
    int* lecture;
}ADC_Monitor_cfg_pkg;

void app_main()
{
    ADC_Monitor_cfg_pkg adc_unit;
    adc_unit.channel = ADC_CHANNEL_0;

    ADC_init_oneshotUnit_and_start(adc_unit.handle_t, adc_unit.channel_config_t);
    // ADC_UNIT_1
    // ADC_ULP_MODE_DISABLE
    // ADC_ATTEN_DB_12
    // BITWIDTH_ADC
    // ADC_CHANNEL_0


    // Start loop
    while(1){
        ADC_oneshot_read(adc_unit.handle_t, adc_unit.channel, adc_unit.lecture);

    }


}

double frequency(){

}