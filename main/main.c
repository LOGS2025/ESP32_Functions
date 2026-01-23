#include "led_gpio.h"
#include "PWM_ledc.h"
#include "PWM_analog.read.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_err.h"

#include "headers.h"

#define PWM_PERIOD_US 20000
#define MAX_DUTY ((1 << 13) - 1)



uint32_t servo_duty_from_us(uint32_t us);
void servo_set_angle(uint32_t angle);

void app_main()
{
    /****************************************************************/
    /********************* INIT PWM GPIO PIN ON *********************/
    /********************* OUTPUT AND SETUP FADE ********************/
    /****************************************************************/

    PWM_ledc_init_timer(50);
    PWM_ledc_init_channel(POT_GPIO,0);
    ledc_fade_func_install(0);
 
    /***************************************************************/
    /********************* INIT UNIT AND HANDLE ********************/
    /***************************************************************/

    ADC_Monitor_cfg_pkg adc_monitor_1;
    ADC_init_unit(&adc_monitor_1.unit_t, &adc_monitor_1.handle_oneshot_t, 0);
    
    /******************************************************/
    /********************* UNIT CONFIG ********************/
    /******************************************************/

    ADC_setup_and_init_channel_and_precision(
        &adc_monitor_1.channel_config_t, 
        &adc_monitor_1.channel, 
        adc_monitor_1.handle_oneshot_t, 
        0
    );

    int raw_Voltage_from_ADC = 0;
    int cali_out_mV_Voltage = 0;

    /*****************************************************/
    /********************** CONFIGURE ********************/
    /********************* CALIBRATION *******************/
    /*****************************************************/

    adc_cali_check_scheme(&adc_monitor_1.scheme);
    ESP_LOGI("cali", "calibration scheme version is %s", "Line Fitting");

    ADC_calib_scheme_line_init_and_start(&adc_monitor_1.cali_adc_config, &adc_monitor_1.handle_cali_adc_unit);

    /******************************************************/
    /********************* READ VALUES ********************/
    /******************************************************/

        /* Uses the raw voltage obtained directly from the oneshot_ADC unit
        and then transforms it into mV units.                            */

    int n = 0;
    int pos_ang = (1 * (180.0/4000));

    while(n<1){
    
            /******************************************************/
            /********************* RAW VOLTAGE ********************/
            /******************************************************/
            
        //ESP_ERROR_CHECK(adc_oneshot_read(
        //    adc_monitor_1.handle_oneshot_t, 
        //    ESP32_ADC_CHANNEL, 
        //    &raw_Voltage_from_ADC)
        //);
        //    
        //    /******************************************************/
        //    /********************* CAL VOLTAGE ********************/
        //    /******************************************************/
//
        //ESP_ERROR_CHECK(adc_cali_raw_to_voltage(
        //        adc_monitor_1.handle_cali_adc_unit, 
        //        raw_Voltage_from_ADC, 
        //        &cali_out_mV_Voltage
        //    )
        //);
//
        //ESP_LOGI("cali", "Raw: %d, Voltage: %d mV", raw_Voltage_from_ADC, cali_out_mV_Voltage);
        //n++;
//
        //printf("Calibration Voltage : %d\n", cali_out_mV_Voltage);
        //pos_ang = cali_out_mV_Voltage * (180.0/4000);
        //printf("Posicion angular: %d\n", pos_ang*10);
        
        //PWM_change_Duty(pos_ang*10);

        servo_set_angle(90);
        esp_Delay(5000);
        printf("Duty : %d ",(PWM_ledc_get_duty()));
        printf("Freq : %d\n",PWM_ledc_get_freq());

        n++;
    }

    //PWM_set_0_Duty_safe(10, 10);

    /*********************************************************************/
    /********************* DELETE CALIBRATION HANDLE  ********************/
    /*********************************************************************/

    ESP_ERROR_CHECK(adc_oneshot_del_unit(adc_monitor_1.handle_oneshot_t));

    ESP_LOGI("cali", "delete %s calibration scheme", "Line Fitting");
    ESP_ERROR_CHECK(adc_cali_delete_scheme_line_fitting(adc_monitor_1.handle_cali_adc_unit));
    ESP_ERROR_CHECK(ledc_stop(LEDC_MODE, LEDC_CHANNEL,0));
    ledc_fade_func_uninstall();

}

/*  SERVO  

Documentation info:
    freq = 50 Hz <- constant
    Duty Cycle = 20 ms square wave <- constant

    1ms = 0º

    1.5ms = 90º

    2ms = 180º

*/

uint32_t servo_duty_from_us(uint32_t us)
{
    return (us * MAX_DUTY) / PWM_PERIOD_US;
}

void servo_set_angle(uint32_t angle)
{
    uint32_t pulse_us;

    if (angle > 180) angle = 180;

    pulse_us = 1000 + (angle * 1000) / 180;  // 1ms → 2ms

    uint32_t duty = servo_duty_from_us(pulse_us);

    ESP_ERROR_CHECK(ledc_set_duty(
        LEDC_LOW_SPEED_MODE,
        LEDC_CHANNEL_0,
        duty
    ));

    ESP_ERROR_CHECK(ledc_update_duty(
        LEDC_LOW_SPEED_MODE,
        LEDC_CHANNEL_0
    ));
}
