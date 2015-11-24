// HC-SR04 ultrasonic distance sensor library

#include <stdint.h>
#include "nrf_gpio.h"
//#include "nrf_timer.h"

//struct representing the HC-SR04 ultrasonic distance sensor
typedef struct HCSR04sensor
{
	int trigger_pin_number;	//pin number for the trigger pin
	int echo_pin_number;	//pin number for the echo pin
//	NRF_TIMER_Type * NRF_TIMERx;	//the timer that this sensor is using for its timing
} HCSR04;

void hcsr04_init(HCSR04 * sensor);

double get_dist(HCSR04 * sensor);

void send_trigger(HCSR04 * sensor);

uint32_t get_high_time(HCSR04 * sensor);

