#include "doorjamb.h"
#include "simple_ble.h"
#include "simple_doorjamb_adv.h"
#include "ble_advdata.h"
#include "simple_adv.h"
#include "led.h"

#define MEASUREMENT_LOWER_BOUND 1000
#define MEASUREMENT_UPPER_BOUND 3000
#define MAX_NUM_MEASUREMENTS 500

volatile int run = 0;
volatile int most_recent_pin = 0;
void pir_callback(nrf_drv_gpiote_pin_t pin_in, nrf_gpiote_polarity_t action)
{
	
	if(run == 0)
		run = 1;
	else
		run = 0;
	most_recent_pin = pin_in;
}

//sets the pins for the distance sensor, but does not config them
void assign_dist_sensors(Doorjamb * door, uint8_t trigger_1, uint8_t echo_1, 
						 sensor_type_t type1, uint8_t trigger_2, uint8_t echo_2,
						 sensor_type_t type2)
{
	door->dist_sensor1.trigger_pin_number = trigger_1;
	door->dist_sensor1.echo_pin_number = echo_1;
	door->dist_sensor1.type = type1;
	door->dist_sensor2.trigger_pin_number = trigger_2;
	door->dist_sensor2.echo_pin_number = echo_2;
	door->dist_sensor2.type = type2;
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
	simple_adv_init();
	simple_adv_start();
	led_init(LED_0);
	led_on(LED_0);
	led_init(LED_1);
	dist_init(&(door->dist_sensor1));
	dist_init(&(door->dist_sensor2));
	pir_init(&(door->pir_1));
	pir_init(&(door->pir_2));
}

//runs the door
void run_door(Doorjamb * door)
{	
	dist_sensor * dist_sensor1 = &(door->dist_sensor1);
	dist_sensor * dist_sensor2 = &(door->dist_sensor2);

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
			for(i = 0; i < MAX_NUM_MEASUREMENTS; ++i)
			{
				dist_vals[i++] = get_dist(dist_sensor1);
				dist_vals[i] = get_dist(dist_sensor2);
			}
			//if run is not set, then another interrupt has occurred
			//if(!run)
			//{
				dir[1] = most_recent_pin;
			//}

			//when done with max number of measurements, or crossing has been called off
			int j = 0;
			volatile uint32_t total_value = 0.0;
			volatile int num_values = 0;
			for(j = 0; j < i; ++j)
			{
				//check if the value is an actual or missed measurement
				if(dist_vals[j] < MEASUREMENT_UPPER_BOUND && MEASUREMENT_LOWER_BOUND < dist_vals[j])
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
			run = 0;

			//determine the direction of the crossing
			action_type_t action;
			if(dir[0] == door->pir_1.gpio_pin)
			{
				if(dir[1] == door->pir_1.gpio_pin)
					action = A_A;
				else
					action = A_B;
			}
			else
			{
				if(dir[1] == door->pir_1.gpio_pin)
					action = B_A;
				else
					action = B_B;
			}
			simple_adv_transaction((uint32_t)average_height, action);
			led_toggle(LED_1);
		}

	}

}
