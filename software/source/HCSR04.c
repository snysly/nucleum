//HC-SR04 ultrasonic sensor library

#include <stdint.h>
#include "HCSR04.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"

void hcsr04_init(HCSR04 * sensor)
{
	nrf_gpio_cfg_output(sensor->trigger_pin_number);
	nrf_gpio_cfg_input(sensor->echo_pin_number, NRF_GPIO_PIN_PULLUP);
}

uint32_t get_dist(HCSR04 * sensor)
{
	send_trigger(sensor);
	return get_high_time(sensor);
}

void send_trigger(HCSR04 * sensor)
{
	//send output to pin
	nrf_gpio_pin_clear(sensor->trigger_pin_number);
	nrf_delay_us(20);
	nrf_gpio_pin_set(sensor->trigger_pin_number);
	nrf_delay_us(12);
	nrf_gpio_pin_clear(sensor->trigger_pin_number);
//	nrf_delay_us(20);
}



uint32_t get_high_time(HCSR04 * sensor)
{
	uint32_t counter = 0;
	uint32_t counter2 = 1;
	while(nrf_gpio_pin_read(sensor->echo_pin_number) == 0 && counter2 < 10000) {++counter2;}
	while(nrf_gpio_pin_read(sensor->echo_pin_number) == 1)
	{
		++counter;
	}
	return counter;
}
