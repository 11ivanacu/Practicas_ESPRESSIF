#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "freertos/timers.h"
#include "driver/adc.h"

#define ledR 33
#define ledG 25
#define ledB 26


uint8_t led_level = 0;
static const char *tag="Main";
TimerHandle_t xTimers;

int interval = 50;
int timerId = 1;
int	adc_val=0;



esp_err_t init_led(void);
esp_err_t blink_led(void);
esp_err_t set_timer(void);
esp_err_t set_adc(void);

void vTimerCallback(TimerHandle_t pxTimer){



	blink_led();

	adc_val = adc1_get_raw(ADC1_CHANNEL_4);
	ESP_LOGI(tag,"ADC VAL: %i", adc_val);
	int adc_case = adc_val/1000; //max =4094
	ESP_LOGI(tag,"ADC VAL: %i", adc_case);

	switch(adc_case){
	case 0:
			gpio_set_level(ledR,0);
			gpio_set_level(ledG,0);
			gpio_set_level(ledB,0);
			break;
	case 1:
			gpio_set_level(ledR,1);
			gpio_set_level(ledG,0);
			gpio_set_level(ledB,0);
			break;
	case 2:
			gpio_set_level(ledR,1);
			gpio_set_level(ledG,1);
			gpio_set_level(ledB,0);
			break;
	case 3:
	case 4:
			gpio_set_level(ledR,1);
			gpio_set_level(ledG,1);
			gpio_set_level(ledB,1);
			break;
	default:
		break;
	}

}

void app_main(void)
{
	init_led();
	set_timer();
	//set_adc();

}


esp_err_t init_led(void)
{
	gpio_reset_pin(ledR);
	gpio_set_direction(ledR, GPIO_MODE_OUTPUT);

	gpio_reset_pin(ledG);
	gpio_set_direction(ledG, GPIO_MODE_OUTPUT);

	gpio_reset_pin(ledB);
	gpio_set_direction(ledB, GPIO_MODE_OUTPUT);

	return ESP_OK;
}

esp_err_t blink_led(void)
{
	led_level =!led_level;
	//gpio_set_level(led1,led_level);
	return ESP_OK;
}

esp_err_t set_timer(void){
	ESP_LOGI(tag, "Timer init configuration");
	xTimers = xTimerCreate("Timer",       // Just a text name, not used by the kernel.
						 	 (pdMS_TO_TICKS(interval)),   // The timer period in ticks.
							 pdTRUE,        // The timers will auto-reload themselves when they expire.
							 (void*)timerId,  // Assign each timer a unique id equal to its array index.
							 vTimerCallback // Each timer calls the same callback when it expires.
	 );

	if( xTimers == NULL )
	{
	  ESP_LOGE(tag, "The timer was not created.");
	}
	else
	{
	  // Start the timer.  No block time is specified, and even if one was
	  // it would be ignored because the scheduler has not yet been
	  // started.
	  if( xTimerStart(xTimers, 0) != pdPASS )
	  {
		  ESP_LOGE(tag,"The timer could not be set into the Active state.");
	  }
	}
	return ESP_OK;
}

esp_err_t set_adc(void){

	adc1_config_channel_atten(ADC1_CHANNEL_4, ADC_ATTEN_DB_11 );		//PIN 32, ATENUACION EN 3.3
	adc1_config_width(ADC_WIDTH_BIT_12);

	return ESP_OK;
}
