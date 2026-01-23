#include "led_gpio.h"
#include "PWM_ledc.h"
#include "PWM_analog.read.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_err.h"

#include "headers.h"

void app_main()
{
        /****************************************************************/
        /********************* INIT PWM GPIO PIN ON *********************/
        /********************* OUTPUT AND SETUP FADE ********************/
        /****************************************************************/

    PWM_ledc_init_timer();
    PWM_ledc_init_channel(POT_GPIO);
    ledc_fade_func_install(0);
 
        /***************************************************************/
        /********************* INIT UNIT AND HANDLE ********************/
        /***************************************************************/

    ADC_Monitor_cfg_pkg adc_monitor_1;
    ADC_init_unit(&adc_monitor_1.unit_t, &adc_monitor_1.handle_t, 0);

        /******************************************************/
        /********************* UNIT CONFIG ********************/
        /******************************************************/

    ADC_setup_and_init_channel_and_precision(
        &adc_monitor_1.channel_config_t, 
        &adc_monitor_1.channel, 
        &adc_monitor_1.handle_t, 
        0
    );

    int raw_Voltage_from_ADC = 0;
    int cali_out_mV_Voltage = 0;

    /*****************************************************/
    /********************** CONFIGURE ********************/
    /********************* CALIBRATION *******************/
    /*****************************************************/

    adc_cali_handle_t handle_cali_adc1_unit1;

    adc_cali_scheme_ver_t scheme;
    adc_cali_check_scheme(&scheme);


    ESP_LOGI("cali", "calibration scheme version is %s", "Line Fitting");

    adc_cali_line_fitting_config_t cali_config = {
        .unit_id = ESP32_ADC_UNIT,
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    ESP_ERROR_CHECK(adc_cali_create_scheme_line_fitting(&cali_config, &handle_cali_adc1_unit1));
  
    /******************************************************/
    /********************* READ VALUES ********************/
    /******************************************************/

        /* Uses the raw voltage obtained directly from the oneshot_ADC unit
        and then transforms it into mV units.                            */

    int n = 0;
    int pos_ang = (1 * (180.0/4000));

    PWM_set_targetDuty_safe(4915, 100, 10);        
    esp_Delay(4000);
    printf("Duty step : %d ",(PWM_ledc_get_duty()));
    printf("Freq step: %d\n",PWM_ledc_get_freq());


    while(n<1){
    
            /******************************************************/
            /********************* RAW VOLTAGE ********************/
            /******************************************************/
            
        ESP_ERROR_CHECK(adc_oneshot_read(
            handle_adc1_unit1, 
            channel, 
            &raw_Voltage_from_ADC)
        );
            
            /******************************************************/
            /********************* CAL VOLTAGE ********************/
            /******************************************************/

        ESP_ERROR_CHECK(adc_cali_raw_to_voltage(
                handle_cali_adc1_unit1, 
                raw_Voltage_from_ADC, 
                &cali_out_mV_Voltage
            )
        );

        ESP_LOGI("cali", "Raw: %d, Voltage: %d mV", raw_Voltage_from_ADC, cali_out_mV_Voltage);
        n++;

        printf("Calibration Voltage : %d\n", cali_out_mV_Voltage);
        pos_ang = cali_out_mV_Voltage * (180.0/4000);
        printf("Posicion angular: %d\n", pos_ang*10);
        
        //PWM_change_Duty(pos_ang*10);

        esp_Delay(1000);
        printf("Duty : %d ",(PWM_ledc_get_duty()));
        printf("Freq : %d\n",PWM_ledc_get_freq());

    }

    //PWM_set_0_Duty_safe(10, 10);

    /*********************************************************************/
    /********************* DELETE CALIBRATION HANDLE  ********************/
    /*********************************************************************/

    ESP_ERROR_CHECK(adc_oneshot_del_unit(handle_adc1_unit1));

    ESP_LOGI("cali", "delete %s calibration scheme", "Line Fitting");
    ESP_ERROR_CHECK(adc_cali_delete_scheme_line_fitting(handle_cali_adc1_unit1));
    ESP_ERROR_CHECK(ledc_stop(LEDC_MODE, LEDC_CHANNEL,0));
    ledc_fade_func_uninstall();

}
