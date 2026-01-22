#include "PWM_ledc.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_adc/adc_oneshot.h"
#include "hal/adc_types.h"

// Calibration for raw voltage
#include "esp_adc/adc_cali.h"

#include "esp_log.h"

#define VMAX 3.3
#define ADC_CHANNEL ADC_CHANNEL_4 // GPIO32

/*

*/

void ADC_init_oneshotUnit_and_start(
    adc_oneshot_unit_handle_t* adc1_handle,
    adc_oneshot_chan_cfg_t* adc_channel_config
);

void ADC_oneshot_delUnit(adc_oneshot_unit_handle_t* adc_handle);

void ADC_oneshot_read(adc_oneshot_unit_handle_t* adc_handle, adc_channel_t* channel, int * raw_lecture);

double ADC_convert_raw(int Dout);
