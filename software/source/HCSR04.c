//HC-SR04 ultrasonic sensor library

#include <stdint.h>
#include "HCSR04.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"
//#include "nrf_timer.h"
/*
void setupTimer(NRF_TIMER_TYPE * timer)
{
	nrf_timer_mode_set(timer, TIMER_MODE_Timer);//set the timer mode to be timer
	nrf_timer_frequency_set(timer, NRF_TIMER_FREQ_1MHz);

	timer->TASKS_CLEAR = 1;
	timer->INTENSET = (TIMER_INTENSET_COMPARE0_Enabled << TIMER_INTENSET_COMPARE0_Pos);
	if(timer == TIMER0)
	{
		NVIC_EnableIRQ(TIMER0_IRQn);
	}
}

void TIMER0_IRQHandler(void)
{
	if(NRF_TIMER0->EVENTS	
}
*/
void hcsr04_init(HCSR04 * sensor)
{
	nrf_gpio_cfg_output(sensor->trigger_pin_number);
	nrf_gpio_cfg_input(sensor->echo_pin_number, NRF_GPIO_PIN_PULLDOWN);
//	NRF_TIMERx = TIMER0;

	//enabling the interrupts
//	setupTimer(NRF_TIMERx);
}

double get_dist(HCSR04 * sensor)
{
	send_trigger(sensor);
	return 0.0;
	uint32_t high_time = get_high_time(sensor);
	double two = high_time;
	return two;
}

void send_trigger(HCSR04 * sensor)
{
	//send output to pin
	
	nrf_gpio_pin_clear(sensor->trigger_pin_number);
	nrf_delay_us(20);
	nrf_gpio_pin_set(sensor->trigger_pin_number);
	nrf_delay_us(12);
	nrf_gpio_pin_clear(sensor->trigger_pin_number);
	nrf_delay_us(20);
}



uint32_t get_high_time(HCSR04 * sensor)
{
	uint32_t counter = 1;
	while(nrf_gpio_pin_read(sensor->echo_pin_number) == 0);
	while(nrf_gpio_pin_read(sensor->echo_pin_number) == 1)
	{
		++counter;
	}
	return counter;
}
