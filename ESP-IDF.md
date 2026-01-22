---
tags:
  - PWM
  - C
---

___
> Application startup covers everything that happens after the app starts executing and before the `app_main` function starts running inside the main task. This is split into three stages:
>  <span style="color:rgb(48, 200, 45)">- Port initialization of hardware and basic C runtime environment.</span>
>     
><span style="color:rgb(255, 192, 0)"> - System initialization of software services and FreeRTOS.</span>
>     
><span style="color:rgb(0, 176, 240)"> - Running the main task and calling `app_main`.</span> 
___
# gpio.h

Included with the header file line:
``` 
#include "driver/gpio.h"
```
## app_main
The function ***is*** allowed to return a value. 

## structure for GPIOs

```c title:"Pin program structure"
static uint8_t s_led_state = 0;

void app_main(){
	// Check pin usable
	if(GPIO_IS_VALIDGPIO(gpio_pin)) continue;
	// Reset mode
	gpio_reset_pin();
	// Set mode
	gpio_set_direction();
	// Here led state changes!!!
	gpio_set_level(..., s_led_state);
	// Delay  
	vTaskDelay();
}
```

### gpio_set_level() | Elegir I/O
For `gpio_set_level(gpio_num_t gpio_num, uint32_t level)`
-> _gpio_num_ is the [^1]**GPIO** number. 
-> _level_ : Output level. `0`: low | `1`: high 
		<span style="color:rgb(0, 176, 240)">We can also get the input level from our pin with</span> #type-int `gpio_get_level(gpio_num_t gpio_num)`:
		-w> the output is either _0 or 1_.
___
### gpio_set_direction() | Definir GPIO : I o O o I&O #mode
For #type-esp_err_t `gpio_set_direction(gpio_num_t gpio_num, gpio_mode_t mofde)`
-> Our **GPIO** and
-> The mode : _output_only | input_only | output_and_input_ 
> [!warning] This function always overwrite all the current modes that have applied on the IO pin

___
### gpio_reset_pin() | Resetear #mode
To reset our pin from whatever mode use #type-esp_err_t `gpio_reset_pin(gpio_num_t gpio_num)`
-> The **GPIO** pin
___
### GPIO_IS_VALID_GPIO() | Verificar GPIO #Macro 
We can also use the #Macro **GPIO_IS_VALID_GPIO**_(gpio_num)_ to check if our pin is valid.
There are also the following: 
#### GPIO_IS_VALID_INPUT_GPIO()
#### GPIO_IS_VALID_OUTPUT_GPIO()
___
### gpio_config() | Inicializar los pasos anteriores en un solo struct

1. `gpio_config(const gpio_config_t *pGPIOConfig)`

> [!warning] This function always overwrite all the current IO configurations

___
# ledc.h

```c title:"ledc.h"
#include "driver/led.h"
```

LEDC channels are divided into **two groups** 
	of _8 chanels each._
One group operates in ***high speed mode***.
The other in ***low speed mode***.
___
$$\text{There are three steps to setup an LEDC channel}$$
___
> [!important] Steps
> 1. [Timer Configuration](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/peripherals/ledc.html#ledc-api-configure-timer) by specifying the _==PWM signal's frequency_== and _==duty cycle resolution==_.
>     
> 2. [Channel Configuration](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/peripherals/ledc.html#ledc-api-configure-channel) by associating it with the _==timer and GPIO==_ to output the PWM signal.
>     
> 3. [Change PWM Signal](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/peripherals/ledc.html#ledc-api-change-pwm-signal) that drives the output in order to change LED's intensity. This can be done _==under the full control of software or with hardware fading functions==_.

___
![[Pasted image 20260120171502.png]]
___
## Timer Configuration
___
### timer_num
_ledc_timer_t_
### speed_mode
_ledc_mode_t_ 
### duty_resolution | frequency in $Hz$
<span style="color:rgb(48, 200, 45)">The frequency and the duty resolution are interdependent. </span>
	**The higher the PWM frequency, the lower the duty resolution which is available.**
	
> [!info] Choosing a duty resolution
> _ledc_find_suitable_duty_resolution()_ : find the maximum possible resolution for the timer, given the ***source clock frequency*** and the desired ***PWM signal frequency***.

#### Timer no longer needed by any channel
	ledc_timer_config()

### clk_cfg | Source Clock
_ledc_clk_cfg_t_
<span style="color:rgb(48, 200, 45)">The source clock can also limit the PWM frequency. </span>
	**Directly proportional to the PWM frequency.**

|  Clock name   | Clock freq |   Speed mode   |                        Clock capabilities                        |
| :-----------: | :--------: | :------------: | :--------------------------------------------------------------: |
|   `APB_CLK`   |  *80 MHz*  | **High / Low** |                              **/**                               |
|  `REF_TICK`   |  *1 MHz*   | **High / Low** |             **Dynamic Frequency Scaling compatible**             |
| `RC_FAST_CLK` | *~ 8 MHz*  |    **Low**     | **Dynamic Frequency Scaling compatible, Light-sleep compatible** |
___
## Channel Configuration #PWM
___
### channel
$$\text{When timer is set up ...}$$
configure the desired channel _(one out of `led_channel_t`)_. By calling :
#### ledc_channel_config()
This function expects a _ledc_channel_config_t_ with the config parameters.


> [!NOTE] When called
> The channel **should** start operating and ***generating the PWM signal on the selected GPIO***.
> With the #frequency specified in the timer settings and the given #duty-cycle.

#### Can be stopped at any time by calling ledc_stop()
___

## Operating mode
### Change signal
When driving #LED's
	<span style="color:rgb(66, 205, 255)">primarily, the</span> #duty-cycle<span style="color:rgb(66, 205, 255)"> is changed to vary the</span> ***light intensity***.

To do this, the options are :w
#### Change PWM Duty Cycle using Software
 **Set**             _ledc_set_duty();_
 **Update**	  _ledc_update_duty();_
 **Check**		  _ledc_get_duty();_

**Set config all in one**      _ledc_get_duty();_

___
 ##### Range if duty cycles
Depends on selected _duty_resolution_ and 
	<span style="color:rgb(48, 200, 45)">should be from</span> $0-2^{(\text{duty\_resolution})}$.
		<span style="color:rgb(48, 200, 45)">except when the channel's binded timer selects its</span> ***maximum duty_resolution***.
___
#### Change PWM Duty Cycle using Hardware
 ### ledc_fade_func_install();
  Enables fading, 
    configure then with :
    *- ledc_set_fade_with_time()*
    *- ledc_set_fade_with_step()*
    *- ledc_set_fade()*
 ### ledc_fade_start();
   Start fading. 
    ***Blocking or Non-Blocking mode*** :
   ### ledc_fade_mode_t();
   Get notification for ***completion of the fade operation*** :
   ### ledc_cb_register();
   Disable with : 
   ### ledc_fade_func_uninstall();
____

## Change PWM Frequency
 ### ledc_set_freq();
 Sets _frequency_.
	To check _frequency_ : 
 ### led_get_freq();

To change the _duty_resolution_ and the _frequency_ 
 ### ledc_bind_channel_timer();
 Change the channel's timer : 
 ### ledc_channel_config();
## Interrupts
 ### ledc_intr_type_t
  triggers and interrupt on fade completion.
  ### ledc_isr_register();

## Program : 

```c title:"ledc_example.c"
void example_ledc_init(void){
	//PWM
	// LEDC PWM timer config
	ledc_timer_config_t ledc_timer = {
		.freq_hz = 4000, // in kHz
		.speed_mode = LEDC_LOW_SPEED_MODE,
		.timer_num = LEDC_TIMER_0,
		.duty_resolution = LEDC_TIMER_13_BIT,
		.clk_cfg = LEDC_AUTO_CLK
	};
	
	ESP_ERROR_CHECK(
		ledc_timer_config(&ledc_timer) // Set timer
	);
	
	// LEDC PWM channel config
	ledc_channel_config_t ledc_channel = {
		.speed_mode = LEDC_LOW_SPEED_MODE,
		.channel = LEDC_CHANNEL_0,
		.timer_sel = LEDC_TIMER_0,
		.intr_type = LEDC_INTR_DISABLE,
		.gpio_num = GPIO_NUM_05,
		.duty = 0, // Set duty to 0%
		.hpoint = 0
	};
	
	ESP_ERROR_CHECK(
		ledc_channel_config(&ledc_channel) // Set channel
	);
}

void app_main()
{
	// Set the LEDC peripheral configuration
	example_ledc_init();
	// Set duty to 50%
	ESP_ERROR_CHECK(
		ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 4096)
		);
	// Update duty to apply the new value
	ESP_ERROR_CHECK(
		ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0)
		);
	  
	return;
}
```

___
# Explanation for PWM
 # Motors
  ## Varying the Work Cycle
   Para motores DC. Variar el #duty-cycle controla la velocidad del motor. 
   A corriente más continua, mayor energía y mayor velocidad.
   ### Why?
   El voltaje proporcionado es constante, $x[V]$, pero al utilizar **PWM**, y convertir en un pulso intermitente, reducimos el voltaje recibido por nuestro motor.
 # Power
  ## Source
   Regular voltaje.
 # LED
  ## Regulate ON time
   Regula la energía gastada en el LED para reducir el desperdicio.
___
## Resolución 
The ESP32 module [has a 1-16 bit resolution](https://www.theengineeringprojects.com/2021/12/esp32-pwm.html). It defines the $$\text{number of steps it can have from zero power to full power.}$$
___

## How to program for a DC motor
 # Mechanical inertia is an important time and efficiency factor
  We will set a duty ramp -> fade to account for this.

![[Pasted image 20260120200732.png]]

## Programming the AD converter for analog input
 # Oneshot
 ### When using ADC_UNIT_1
  We get a table like this, that maps each channel to each GPIO.

| ADC Channel   | GPIO   |
| ------------- | ------ |
| ADC_CHANNEL_0 | GPIO36 |
| ADC_CHANNEL_1 | GPIO37 |
| ADC_CHANNEL_2 | GPIO38 |
| ADC_CHANNEL_3 | GPIO39 |
| ADC_CHANNEL_4 | GPIO32 |
| ADC_CHANNEL_5 | GPIO33 |
| ADC_CHANNEL_6 | GPIO34 |
| ADC_CHANNEL_7 | GPIO35 |
 # Calibration
 ADC calibration is given by the _handle_ **adc_cali_handle_t**.
  ### adc_cali_check_scheme()
   Returns the supported calibration schemes for the chip.
  ### adc_cali_check_scheme()
   Checks the calibration scheme given by the user.
 
 ## Line Fitting Scheme
This uses the same configuration worked by _oneshot structs_:
	We need to fill the next:
- unit_id -> the ADC
- atten -> attenuation the raw results use
- bitwidth 
 ### eFuse?
 

___
[^1]: GPIO : 
	General Purpose Input/Output. [GPIO](https://en.wikipedia.org/wiki/General-purpose_input/output)
