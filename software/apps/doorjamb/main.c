#include <stdint.h>
#include <stdbool.h>
#include "HCSR04.h"
#include "ParallaxPIR.h"

//define parameters for the pins things are connected to
#define HCSR04_TRIGGER_1
#define HCSR04_ECHO_1
#define HCSR04_TRIGGER_2
#define HCSR04_ECHO_2

#define PIR_1_INPUT
#define PIR_2_INPUT

//miscellaneous macros needed
//maximum number of measurements that can be taken by distance sensors
#define MAX_NUM_MEASUREMENTS
//lower bound for deciding if the distance measurement measured no person
#define NULL_MEASUREMENT_LOWER_BOUND

bool run = false;
int most_recent_pin;
void pir_callback(nrf_drv_gpiote_pin_t pin_in, nrf_gpiote_polarity_t action)
{
	run = !run;
	most_recent_pin = pin_in;
}

int main(void)
{
	//initialize distance sensors
	HCSR04 dist_sensor1;
	dist_sensor1.trigger_pin_number = HCSR04_TRIGGER_1;
	dist_sensor1.echo_pin_number = HCSR04_ECHO_1;
	hcsr04_init(&dist_sensor1);

	HCSR04 dist_sensor2;
	dist_sensor2.trigger_pin_number = HCSR04_TRIGGER_2;
	dist_sensor2.echo_pin_number = HCSR04_ECHO_2;
	hcsr04_init(&dist_sensor2);

	//initialize PIR Sensors
	ParallaxPIR pir_1;
	pir_1.gpio_pin = PIR_1_INPUT;
	pir_1.callback_function = pir_callback;
	pir_init(&pir_1);

	ParallaxPIR pir_2;
	pir_2.gpio_pin = PIR_2_INPUT;
	pir_2.callback_function = pir_callback;
	pir_init(&pir_2);

	//intialize data collection 
	double dist_vals[MAX_NUM_MEASUREMENTS];
	int dir[2];
	//actual loops
	while(1)
	{
		if(run)
		{
			//record which side of the door is most recent
			dir[0] = most_recent_pin;
			
			int i = 0;
			for(i = 0; i < MAX_NUM_MEASUREMENTS && run; ++i)
			{
				dist_vals[i++] = get_dist(&dist_sensor1);
				dist_vals[i] = get_dist(&dist_sensor2);
			}
			//if run is not set, then another interrupt has occurred
			if(!run)
			{
				dir[1] = most_recent_pin;
			}

			//when done with max number of measurements, or crossing has been called off
			int j = 0;
			double total_value = 0.0;
			int num_values = 0;
			for(j = 0; j < i; ++j)
			{
				//check if the value is an actual or missed measurement
				if(dist_vals[j] < NULL_MEASUEMENT_LOWER_BOUND)
				{
					//if it is an actual measurement, record data for averaging
					total_value += dist_vals[j];
					++num_values;
				}

				//zero out the distance values to prevent future problems
				dist_vals[j] = 0.0;
			}

			//get the average for sending
			double average_height = total_value/((double)num_values);
			run = false;
		}

	}
}
