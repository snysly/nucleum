#ifndef __PARALLAX_PIR_H__
#define __PARALLAX_PIR_H__

#include <stdint.h>
#include "nrf_gpiote.h"
#include "nrf_drv_gpiote.h"

typedef struct
{
	uint32_t gpio_pin;
	nrf_drv_gpiote_evt_handler_t callback_function;
} ParallaxPIR;

//initializes the pir sensor associated with pin pin_in
//this sensor will now call callback whenever pin_in goes from low to high
//configures the gpiote event as a port event
void pir_init(ParallaxPIR * sensor);
#endif
