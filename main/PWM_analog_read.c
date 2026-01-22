#include "PWM_analog.read.h"

/*
ADC1:
8 channels: GPIO32 - GPIO39

ADC2:
10 channels: GPIO0, GPIO2, GPIO4, GPIO12 - GPIO15, GOIO25 - GPIO27

ADC_ATTEN_DB_11

150 mV ~ 2450 mV
*/


// Init unit, handle, channel and config
void ADC_init_oneshotUnit_and_start(
    adc_oneshot_unit_handle_t* adc1_handle,
    adc_oneshot_chan_cfg_t* adc_channel_config
)
{
    adc_oneshot_unit_init_cfg_t init_config1 = {
        .clk_src = 0,
        .unit_id = ADC_UNIT_1,// or ADC_UNIT_2
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config1, &adc1_handle));

    adc_channel_config->atten = ADC_ATTEN_DB_12;
    adc_channel_config->bitwidth = ADC_BITWIDTH_DEFAULT;

    // Set this up
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_0 ,&adc_channel_config));
}

// Delete unit
void ADC_oneshot_delUnit(adc_oneshot_unit_handle_t* adc_handle){
    ESP_ERROR_CHECK(adc_oneshot_del_unit(adc_handle));
}

// Check readings
void ADC_oneshot_read(
    adc_oneshot_unit_handle_t* adc_handle, 
    adc_channel_t* channel, 
    int * raw_lecture)
{
    ESP_ERROR_CHECK(adc_oneshot_read(adc_handle, channel, raw_lecture));
}

// Convert raw lecture
double ADC_convert_raw(int Dout){
    return Dout * VMAX / (2^BITWIDTH_ADC);
}