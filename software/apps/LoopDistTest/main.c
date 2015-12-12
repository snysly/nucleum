//Program for testing the HCSR04 sensor
#include <stdint.h>
#include <stdlib.h>
#include "dist_sensor.h"

#define TRIGGER_PIN 23
#define ECHO_PIN 24
#define MAX_NUM_MEASUREMENTS 500
#define LOWER_BOUND 1000
#define UPPER_BOUND 3000

int main(void)
{
	dist_sensor sensor;
	sensor.trigger_pin_number = TRIGGER_PIN;
	sensor.echo_pin_number = ECHO_PIN;
	sensor.type = HCSR04;
	dist_init(&sensor);

	volatile uint32_t dist_vals[MAX_NUM_MEASUREMENTS];
	for(int i = 0; i < MAX_NUM_MEASUREMENTS; ++i)
	{
		dist_vals[i] = get_dist(&sensor);
	}

	uint32_t num_val = 0;
	uint32_t total = 0;
	for(int i = 0; i < MAX_NUM_MEASUREMENTS; ++i)
	{
		if(dist_vals[i] < UPPER_BOUND && LOWER_BOUND < dist_vals[i])
		{
			++num_val;
			total += dist_vals[i];
		}
	}
	double result = (double)total/(double)num_val;
	printf("result: %f\n", result);
	return 0;
}
