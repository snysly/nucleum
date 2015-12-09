// HC-SR04 ultrasonic distance sensor library

#include <stdint.h>

//struct representing the HC-SR04 ultrasonic distance sensor
typedef struct HCSR04sensor
{
	uint8_t trigger_pin_number;	//pin number for the trigger pin
	uint8_t echo_pin_number;	//pin number for the echo pin
} HCSR04;

void hcsr04_init(HCSR04 * sensor);

uint32_t get_dist(HCSR04 * sensor);

void send_trigger(HCSR04 * sensor);

uint32_t get_high_time(HCSR04 * sensor);

