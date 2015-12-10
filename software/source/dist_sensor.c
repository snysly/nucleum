//HC-SR04 ultrasonic sensor library

#include <stdint.h>
#include "dist_sensor.h"
#include "nrf_gpio.h"
#include "nrf_delay.h"

void dist_init(dist_sensor * sensor)
{
	nrf_gpio_cfg_output(sensor->trigger_pin_number);
	nrf_gpio_cfg_input(sensor->echo_pin_number, NRF_GPIO_PIN_NOPULL);
	switch(sensor->type)
	{
		case HCSR04:	sensor->trigger_time = 12;
						break;
		case MAXBOTIX:	sensor->trigger_time = 22;
						break;
	}
}

uint32_t get_dist(dist_sensor * sensor)
{
	send_trigger(sensor);
	return get_high_time(sensor);
}

void send_trigger(dist_sensor * sensor)
{
	//send output to pin
	nrf_gpio_pin_clear(sensor->trigger_pin_number);
	nrf_delay_us(20);
	nrf_gpio_pin_set(sensor->trigger_pin_number);
	nrf_delay_us(sensor->trigger_time);
	nrf_gpio_pin_clear(sensor->trigger_pin_number);
//	nrf_delay_us(20);
}



uint32_t get_high_time(dist_sensor * sensor)
{
	uint32_t counter = 0;
	uint32_t counter2 = 1;
	while(nrf_gpio_pin_read(sensor->echo_pin_number) == 0 && counter2 < 10000) {++counter2;}
	while(nrf_gpio_pin_read(sensor->echo_pin_number) == 1 && counter < 10000)
	{
		++counter;
	}
	return counter;
}
