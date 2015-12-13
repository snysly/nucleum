#include <stdint.h>
#include "doorjamb.h"

//define parameters for the pins things are connected to
#define DIST_SENSOR_TRIGGER_1 24
#define DIST_SENSOR_ECHO_1	23
#define DIST_SENSOR_TRIGGER_2 21
#define DIST_SENSOR_ECHO_2 22

#define PIR_1_INPUT 9
#define PIR_2_INPUT 10

/*
volatile int run = false;
volatile int most_recent_pin;
void pir_callback(nrf_drv_gpiote_pin_t pin_in, nrf_gpiote_polarity_t action)
{
	run = !run;
	most_recent_pin = pin_in;
}
*/

int main(void)
{
	//create a new doorjamb
	Doorjamb door;

	//assign the pin numbers and the door id
	assign_dist_sensors(&door, DIST_SENSOR_TRIGGER_1, DIST_SENSOR_ECHO_1, MAXBOTIX,
							DIST_SENSOR_TRIGGER_2, DIST_SENSOR_ECHO_2, MAXBOTIX);

	assign_pir_sensors(&door, PIR_1_INPUT, pir_callback, PIR_2_INPUT, pir_callback);

	//initialize the ble and the sensors
	doorjamb_init(&door);

	//run the door infinitely
	run_door(&door);
}
/*
	//initialize distance sensors
	DIST_SENSOR dist_sensor1;
	dist_sensor1.trigger_pin_number = DIST_SENSOR_TRIGGER_1;
	dist_sensor1.echo_pin_number = DIST_SENSOR_ECHO_1;
	hcsr04_init(&dist_sensor1);

	DIST_SENSOR dist_sensor2;
	dist_sensor2.trigger_pin_number = DIST_SENSOR_TRIGGER_2;
	dist_sensor2.echo_pin_number = DIST_SENSOR_ECHO_2;
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
	volatile double dist_vals[MAX_NUM_MEASUREMENTS];
	volatile int dir[2];
	//actual loops
	while(1)
	{
		if(run == 1)
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
			volatile double total_value = 0.0;
			volatile int num_values = 0;
			for(j = 0; j < i; ++j)
			{
				//check if the value is an actual or missed measurement
				if(dist_vals[j] < NULL_MEASUREMENT_LOWER_BOUND)
				{
					//if it is an actual measurement, record data for averaging
					total_value += dist_vals[j];
					++num_values;
				}

				//zero out the distance values to prevent future problems
			//`	dist_vals[j] = 0.0;
			}

			//get the average for sending
			double average_height = total_value/((double)num_values);
			run = false;
		}

	}
}
*/
