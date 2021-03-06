#ifndef __DOORJAMB_H__
#define __DOORJAMB_H__

#include "ParallaxPIR.h"
#include "dist_sensor.h"
#include "ble.h"
#include "simple_ble.h"
#include "ble_advdata.h"
#include "ble_debug_assert_handler.h"
#include "simple_doorjamb_adv.h"
#include "boards.h"

typedef nrf_drv_gpiote_evt_handler_t callback_func;
typedef struct {
	ParallaxPIR pir_1;
	ParallaxPIR pir_2;
	dist_sensor dist_sensor1;
	dist_sensor dist_sensor2;
} Doorjamb;

void pir_callback(nrf_drv_gpiote_pin_t pin_in, nrf_gpiote_polarity_t action);

//call the assign functions, then the init function before running the door
//sets the pins for the distance sensor, but does not config them
void assign_dist_sensors(Doorjamb * door, uint8_t trigger_1, uint8_t echo_1, sensor_type_t type1, uint8_t trigger_2, uint8_t echo_2, sensor_type_t type2);

//sets the pins for the pir sensors, but does not config them
void assign_pir_sensors(Doorjamb * door, uint32_t gpio_1, callback_func func_1, uint32_t gpio_2, callback_func func_2); 

//configures all the sensors and the ble
void doorjamb_init(Doorjamb * door);

//runs the door
void run_door(Doorjamb * door);
#endif
