//HC-SR04 ultrasonic sensor library
// the LP stands for Low Power

#include <stdint.h>
#include "HCSR04LP.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"

void hcsr04_init(HCSR04LP * sensor)
{
	nrf_gpio_cfg_output(sensor->trigger_pin_number);
	nrf_gpio_cfg_input(sensor->echo_pin_number, NRF_GPIO_PIN_NOPULL);
	nrf_gpio_cfg_output(sensor->power_pin_number);
}

double get_dist(HCSR04LP * sensor)
{
	send_trigger(sensor);
	return get_high_time(sensor);
}

void send_trigger(HCSR04LP * sensor)
{
	//send output to pin
	nrf_gpio_pin_clear(sensor->trigger_pin_number);
	nrf_delay_us(20);
	nrf_gpio_pin_set(sensor->trigger_pin_number);
	nrf_delay_us(12);
	nrf_gpio_pin_clear(sensor->trigger_pin_number);
}



uint32_t get_high_time(HCSR04LP * sensor)
{
	uint32_t counter = 1;
	while(nrf_gpio_pin_read(sensor->echo_pin_number) == 0);
	while(nrf_gpio_pin_read(sensor->echo_pin_number) == 1)
	{
		++counter;
	}
	return counter;
}

void turn_on(HCSR04LP * sensor)
{
	nrf_gpio_pin_set(sensor->power_pin_number);
}

void turn_off(HCSR04LP * sensor)
{
	nrf_gpio_pin_clear(sensor->power_pin_number);
}

