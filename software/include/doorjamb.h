#ifndef __DOORJAMB_H__
#define __DOORJAMB_H__

#include "ParallaxPIR.h"
#include "HCSR04.h"
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
	HCSR04 dist_sensor1;
	HCSR04 dist_sensor2;
	uint8_t door_id;
} Doorjamb;

// Intervals for advertising and connections
static simple_ble_config_t ble_config = {
    .platform_id       = 0x80,              // used as 4th octect in device BLE address
    .device_id         = DEVICE_ID_DEFAULT,
    .adv_name          = DEVICE_NAME,       // used in advertisements if there is room
    .adv_interval      = MSEC_TO_UNITS(100, UNIT_0_625_MS),
    .min_conn_interval = MSEC_TO_UNITS(500, UNIT_1_25_MS),
    .max_conn_interval = MSEC_TO_UNITS(1000, UNIT_1_25_MS)
};

void pir_callback(nrf_drv_gpiote_pin_t pin_in, nrf_gpiote_polarity_t action);

//call the assign functions, then the init function before running the door
//sets the pins for the distance sensor, but does not config them
void assign_dist_sensors(Doorjamb * door, uint8_t trigger_1, uint8_t echo_1, uint8_t trigger_2, uint8_t echo_2);

//sets the pins for the pir sensors, but does not config them
void assign_pir_sensors(Doorjamb * door, uint32_t gpio_1, callback_func func_1, uint32_t gpio_2, callback_func func_2); 

//configures all the sensors and the ble
void doorjamb_init(Doorjamb * door);

//runs the door
void run_door(Doorjamb * door);
#endif
