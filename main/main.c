#include "led_gpio.h"
#include "PWM_ledc.h"
#include "PWM_analog.read.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_err.h"

#include "headers.h"

int SERVO_interpol_duty(int angulo);
int PWM_SERVO_interpol(int voltage);
void SERVO_angulo_move(int angulo);
void SERVO_angulo_move_volt(int voltage);


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
    while(n<120){
    
            /******************************************************/
            /********************* RAW VOLTAGE ********************/
            /******************************************************/
            
        ESP_ERROR_CHECK(adc_oneshot_read(
            adc_monitor_1.handle_oneshot_t, 
            ESP32_ADC_CHANNEL, 
            &raw_Voltage_from_ADC)
        );
            
            /******************************************************/
            /********************* CAL VOLTAGE ********************/
            /******************************************************/

        ESP_ERROR_CHECK(adc_cali_raw_to_voltage(
                adc_monitor_1.handle_cali_adc_unit, 
                raw_Voltage_from_ADC, 
                &cali_out_mV_Voltage
            )
        );

        ESP_LOGI("cali", "Raw: %d, Voltage: %d mV", raw_Voltage_from_ADC, cali_out_mV_Voltage);
        n++;

        printf("Calibration Voltage : %d\n", cali_out_mV_Voltage);      

        SERVO_angulo_move_volt(cali_out_mV_Voltage);
        printf("Duty : %d ",(PWM_ledc_get_duty()));
        printf("Freq : %d\n",PWM_ledc_get_freq());

        n++;
    }

    /*********************************************************************/
    /********************* DELETE CALIBRATION HANDLE  ********************/
    /*********************************************************************/

    ESP_ERROR_CHECK(adc_oneshot_del_unit(adc_monitor_1.handle_oneshot_t));

    ESP_LOGI("cali", "delete %s calibration scheme", "Line Fitting");
    ESP_ERROR_CHECK(adc_cali_delete_scheme_line_fitting(adc_monitor_1.handle_cali_adc_unit));
    ESP_ERROR_CHECK(ledc_stop(LEDC_MODE, LEDC_CHANNEL,0));
    ledc_fade_func_uninstall();

}

    // Para interpolar -> 614 = 90º | >409 = 0º | <819 = 180º
    // Para interpolar -> 600 = 90º | 300 = 0º | 1000 = 180º
int SERVO_interpol_duty(int angulo){
    // Punto 1 x = 0º | y = 300
    // Punto 2 x = 180º | y = 1000
    return ((1000 - 300)/(180.0)) * (angulo) + 300;
}

int PWM_SERVO_interpol(int voltage){
    // Punto 1 x = 140mV | y = 0º
    // Punto 2 x = 3150mV | y = 180º
    return ((180.0)/(3150-140)) * (voltage - 140);
}

void SERVO_angulo_move(int angulo){
    PWM_change_Duty(SERVO_interpol_duty(angulo));
    esp_Delay(500);
}

void SERVO_angulo_move_volt(int voltage){
    PWM_change_Duty(SERVO_interpol_duty(PWM_SERVO_interpol(voltage)));
    esp_Delay(500);
}
