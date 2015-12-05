#include "utils.h"
#include "ParallaxPIR.h"
#include "nrf_drv_gpiote.h"
#include <stdbool.h>

void pir_init(ParallaxPIR * sensor)
{
	//initialize the gpiote events,
	gpiote_init();

	//initialize the pin as a gpiote pin
	nrf_drv_gpiote_in_config_t config = GPIOTE_CONFIG_IN_SENSE_LOTOHI(true);
	config.pull = NRF_GPIO_PIN_NOPULL;
	nrf_drv_gpiote_in_init(sensor->gpio_pin, &config, sensor->callback_function);
	nrf_drv_gpiote_in_event_enable(sensor->gpio_pin, true);
}
