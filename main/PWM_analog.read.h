#include "PWM_ledc.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

    #include "esp_adc/adc_oneshot.h"
        #include "hal/adc_types.h"

    // Calibration for raw voltage
    #include "esp_adc/adc_cali.h"

    #include "esp_log.h"
        #include "esp_log_buffer.h" // ESP_LOGI macro whic spits Information messages which describe normal flow of events
        #include "esp_log_level.h"

#define VMAX 3.3

typedef struct ADC_Monitor_cfg_pkg {
    adc_oneshot_unit_init_cfg_t unit_t; // Unit
    adc_oneshot_unit_handle_t handle_oneshot_t; // Handle
    adc_oneshot_chan_cfg_t channel_config_t; // Channel config
    adc_channel_t channel; // Channel

    // Calibration part
    adc_cali_handle_t handle_cali_adc_unit;
    adc_cali_line_fitting_config_t cali_adc_config;
    adc_cali_scheme_ver_t scheme;
    
    // Store values
    int cali_out_mV_Samples[BUFFER_SAMPLES];

}ADC_Monitor_cfg_pkg;

void ADC_init_unit(adc_oneshot_unit_init_cfg_t* unit_adc, adc_oneshot_unit_handle_t* handle_adc_unit, int flag_change_default);

void ADC_setup_and_init_channel_and_precision(adc_oneshot_chan_cfg_t* channel_config, adc_channel_t* channel, adc_oneshot_unit_handle_t handle_adc_unit, int default_flag);

void ADC_calib_scheme_line_init_and_start(adc_cali_line_fitting_config_t* cali_config, adc_cali_handle_t* handle_cali_adc_unit);
