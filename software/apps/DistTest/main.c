//Program for testing the HCSR04 sensor
#include <stdint.h>
#include "dist_sensor.h"

#define TRIGGER_PIN 23
#define ECHO_PIN 24

int main(void)
{
	dist_sensor sensor;
	sensor.trigger_pin_number = TRIGGER_PIN;
	sensor.echo_pin_number = ECHO_PIN;
	sensor.type = MAXBOTIX;
	dist_init(&sensor);

	double result = get_dist(&sensor);
	return 0;
}
