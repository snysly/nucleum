#include <stdint.h>
#include "HCSR04LP.h"
#include "nrf_delay.h"

#define TRIGGER_PIN 23
#define ECHO_PIN 24
#define POWER_PIN 22

int main(void)
{
	HCSR04LP sensor;
	sensor.trigger_pin_number = TRIGGER_PIN;
	sensor.echo_pin_number = ECHO_PIN;
	sensor.power_pin_number = POWER_PIN;

	hcsr04_init(&sensor);
	turn_on(&sensor);
	nrf_delay_us(1000000);

	volatile double result = 0.0;
	result = get_dist(&sensor);
	turn_off(&sensor);
	return (int)result;
}
