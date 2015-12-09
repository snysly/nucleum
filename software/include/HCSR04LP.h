// HC-SR04 ultrasonic distance sensor library
// The LP stands for low power

#include <stdint.h>

//struct representing the HC-SR04 ultrasonic distance sensor
typedef struct HCSR04sensorLP
{
	int trigger_pin_number;	//pin number for the trigger pin
	int echo_pin_number;	//pin number for the echo pin
	int power_pin_number;	// the pin number for the power
} HCSR04LP;

void hcsr04_init(HCSR04LP* sensor);

double get_dist(HCSR04LP* sensor);

void send_trigger(HCSR04LP* sensor);

uint32_t get_high_time(HCSR04LP* sensor);

void turn_on(HCSR04LP * sensor);

void turn_off(HCSR04LP * sensor);

