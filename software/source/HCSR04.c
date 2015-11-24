//HC-SR04 ultrasonic sensor library

#include <stdint.h>
#include "HCSR04.h"
#include "nrf_gpio.h"
#include "nrf_timer.h"

void hcsr04_init(HCSR04 * sensor)
{
	nrf_gpio_cfg_output(sensor->trigger_pin_number);
	nrf_gpio_cfg_input(sensor->echo_pin_number);
}

double get_dist(HCSR04 * sensor)
{
	send_trigger(sensor);
	uint32_t high_time = get_high_time(sensor);
	
}

void send_trigger(HCSR04 * sensor)
{
	
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
