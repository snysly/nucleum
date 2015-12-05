#include "utils.h"
#include "nrf_drv_gpiote.h"

uint32_t gpiote_init()
{
	if(!nrf_drv_gpiote_is_init())
		return nrf_drv_gpiote_init();
	return 0;
}
