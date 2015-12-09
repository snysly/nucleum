#include "doorjamb.h"
#define NULL_MEASUREMENT_LOWER_BOUND 20000
#define MAX_NUM_MEASUREMENTS 1000

int run;
int most_recent_pin;
void pir_callback(nrf_drv_gpiote_pin_t pin_in, nrf_gpiote_polarity_t action)
{
	run = !run;
	most_recent_pin = pin_in;
}

//sets the pins for the distance sensor, but does not config them
void assign_dist_sensors(Doorjamb * door, uint8_t trigger_1, uint8_t echo_1,
						 uint8_t trigger_2, uint8_t echo_2)
{
	door->dist_sensor1.trigger_pin_number = trigger_1;
	door->dist_sensor1.echo_pin_number = echo_1;
	door->dist_sensor2.trigger_pin_number = trigger_2;
	door->dist_sensor2.echo_pin_number = echo_2;
}

//sets the pins for the pir sensors, but does not config them
void assign_pir_sensors(Doorjamb * door, uint32_t gpio_1, callback_func func_1,
						uint32_t gpio_2, callback_func func_2) 
{
	door->pir_1.gpio_pin = gpio_1;
	door->pir_1.callback_function = func_1;
	door->pir_2.gpio_pin = gpio_2;
	door->pir_2.callback_function = func_2;
}

//configures all the sensors and the ble
void doorjamb_init(Doorjamb * door)
{
	hcsr04_init(&(door->dist_sensor1));
	hcsr04_init(&(door->dist_sensor2));
	pir_init(&(door->pir_1));
	pir_init(&(door->pir_2));
}

extern int run;
extern int most_recent_pin;

//runs the door
void run_door(Doorjamb * door)
{	
	HCSR04 * dist_sensor1 = &(door->dist_sensor1);
	HCSR04 * dist_sensor2 = &(door->dist_sensor2);

	//intialize data collection 
	volatile uint32_t dist_vals[MAX_NUM_MEASUREMENTS];
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
				dist_vals[i++] = get_dist(dist_sensor1);
				dist_vals[i] = get_dist(dist_sensor2);
			}
			//if run is not set, then another interrupt has occurred
			if(!run)
			{
				dir[1] = most_recent_pin;
			}

			//when done with max number of measurements, or crossing has been called off
			int j = 0;
			volatile uint32_t total_value = 0.0;
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
			double average_height = (double)total_value/((double)num_values);
			run = false;
		}

	}

}
