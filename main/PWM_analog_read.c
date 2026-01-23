#include "PWM_analog.read.h"

/***************************************************************/
/********************* INIT UNIT AND HANDLE ********************/
/***************************************************************/

    // default use flag 0
void ADC_init_unit(adc_oneshot_unit_init_cfg_t* unit_adc, adc_oneshot_unit_handle_t* handle_adc_unit, int flag_change_default){
    
    if(flag_change_default != 0){
        printf("Put different values inside ADC_init_unit\n");
        return;
    }
    else{
        unit_adc->clk_src = 0; // default if not modified  
        unit_adc->ulp_mode = ADC_ULP_MODE_DISABLE; // coprocessor
        unit_adc->unit_id = ESP32_ADC_UNIT;
        printf("Finished init ADC function\n");
    
        ESP_ERROR_CHECK(adc_oneshot_new_unit(
        unit_adc,
        handle_adc_unit)
    );
    }
}

/******************************************************/
/********************* UNIT CONFIG ********************/
/******************************************************/

void ADC_setup_and_init_channel_and_precision(adc_oneshot_chan_cfg_t* channel_config, adc_channel_t* channel, adc_oneshot_unit_handle_t handle_adc_unit, int default_flag){
    if(default_flag == 0){
        channel_config->atten = ADC_ATTEN_DB_12;
        channel_config->bitwidth = ADC_BITWIDTH_DEFAULT;

        // Defined in macro

        ESP_ERROR_CHECK(adc_oneshot_config_channel(
        handle_adc_unit, 
        ESP32_ADC_CHANNEL, 
        channel_config)
    );
    } 
    else {
        printf("Didnt set default\n");
        return;
    }
}

/*****************************************************/
/********************** CONFIGURE ********************/
/********************* CALIBRATION *******************/
/*****************************************************/

void ADC_calib_scheme_line_init_and_start(adc_cali_line_fitting_config_t* cali_config, adc_cali_handle_t* handle_cali_adc_unit){
    cali_config->atten = ADC_ATTEN_DB_12;
    cali_config->unit_id = ESP32_ADC_UNIT;
    cali_config->bitwidth = ADC_BITWIDTH_DEFAULT;

    ESP_ERROR_CHECK(adc_cali_create_scheme_line_fitting(cali_config, handle_cali_adc_unit));
}
