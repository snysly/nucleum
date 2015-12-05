#include "ParallaxPIR.h"

#define GPIO_PIN_IN 8

void pir_test_call(nrf_drv_gpiote_pin_t pin_in, nrf_gpiote_polarity_t action)
{
	//while(1)
	//{
	//	bool result = nrf_drv_gpiote_in_is_set(pin_in);
	//}
}

int main(void)
{
	ParallaxPIR sensor;
	sensor.gpio_pin = GPIO_PIN_IN;
	sensor.callback_function = pir_test_call;

	pir_init(&sensor);
	while(1)
	{int i = 0;}
	return 0;
}
