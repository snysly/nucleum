// HC-SR04 ultrasonic distance sensor library

#include <stdint.h>
#include "nrf_gpio.h"

typedef struct HCSR04sensor
{
	int trigger_pin_number;
	int echo_pin_number;
}; HCSR04

void hcsr04_init(HCSR04 * sensor);

double get_dist(HCSR04 * sensor);

void send_trigger(HCSR04 * sensor);

uint32_t get_high_time(HCSR04 * sensor);

