#ifndef __DIST_SENSOR_H__
#define __DIST_SENSOR_H__
//ultrasonic distance sensor library
// this relies on distance sensors outputing a pulse whose width is relative to thedistance measured
// it will return a number that increases as the width of that pulse increases
#include <stdint.h>

typedef enum {HCSR04, MAXBOTIX} sensor_type_t;

//struct representing the HC-SR04 ultrasonic distance sensor
typedef struct distsensor
{
	uint8_t trigger_pin_number;	//pin number for the trigger pin
	uint8_t echo_pin_number;	//pin number for the echo pin
	uint8_t trigger_time;	//the number of microseconds to pulse the trigger for
	sensor_type_t type;
} dist_sensor;

void dist_init(dist_sensor * sensor);

uint32_t get_dist(dist_sensor * sensor);

void send_trigger(dist_sensor * sensor);

uint32_t get_high_time(dist_sensor * sensor);
#endif
