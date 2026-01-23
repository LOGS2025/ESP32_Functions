/*******************************************************************/
/********************* MACRO FOR PWM POT GPIO **********************/
/*******************************************************************/

#define POT_GPIO GPIO_NUM_5 // Define the output GPIO
#define LEDC_TIMER              LEDC_TIMER_0
#define LEDC_MODE               LEDC_LOW_SPEED_MODE
#define LEDC_CHANNEL            LEDC_CHANNEL_0
#define LEDC_DUTY_RES           LEDC_TIMER_13_BIT // Set duty resolution to 13 bits
#define LEDC_MAX_DUTY           (409.6) // Set duty to 50%. (2 ** 13) * 50% = 4096
#define LEDC_FREQUENCY          (50) // Frequency in Hertz. Set frequency at 4 kHz

/*******************************************************************/
/********************* MACRO FOR ADC - CHANNEL *********************/
/*******************************************************************/

#define ADC_PIN_36

#ifdef ADC_PIN_36
    #define ESP32_ADC_UNIT ADC_UNIT_1
    #define ESP32_ADC_CHANNEL ADC_CHANNEL_0
#endif
#ifdef ADC_PIN_37
    #define ESP32_ADC_UNIT ADC_UNIT_1
    #define ESP32_ADC_CHANNEL ADC_CHANNEL_1
#endif
#ifdef ADC_PIN_38
    #define ESP32_ADC_UNIT ADC_UNIT_1
    #define ESP32_ADC_CHANNEL ADC_CHANNEL_2
#endif
#ifdef ADC_PIN_39
    #define ESP32_ADC_UNIT ADC_UNIT_1
    #define ESP32_ADC_CHANNEL ADC_CHANNEL_3
#endif
#ifdef ADC_PIN_32
    #define ESP32_ADC_UNIT ADC_UNIT_1
    #define ESP32_ADC_CHANNEL ADC_CHANNEL_4
#endif
#ifdef ADC_PIN_33
    #define ESP32_ADC_UNIT ADC_UNIT_1
    #define ESP32_ADC_CHANNEL ADC_CHANNEL_5
#endif
#ifdef ADC_PIN_34
    #define ESP32_ADC_UNIT ADC_UNIT_1
    #define ESP32_ADC_CHANNEL ADC_CHANNEL_5
#endif
#ifdef ADC_PIN_35
    #define ESP32_ADC_UNIT ADC_UNIT_1
    #define ESP32_ADC_CHANNEL ADC_CHANNEL_7
#endif

/*******************************************************************/
/********************* MACRO FOR ADC ATTENUATION *******************/
/*******************************************************************/

/*
ADC1:
8 channels: GPIO32 - GPIO39

ADC2:
10 channels: GPIO0, GPIO2, GPIO4, GPIO12 - GPIO15, GOIO25 - GPIO27

ADC_ATTEN_DB_11

150 mV ~ 2450 mV
*/
