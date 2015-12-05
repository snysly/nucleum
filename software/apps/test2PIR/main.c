#include "ParallaxPIR.h"
#include "nrf_gpio.h"
#define GPIO_PIN_IN1 8
#define GPIO_PIN_IN2 9

int counter = 0;
int run = 0;
void pir_test_call(nrf_drv_gpiote_pin_t pin_in, nrf_gpiote_polarity_t action)
{
	run = !run;
	if(counter == 2)
		counter = 3;
	else if(counter == 0)
		counter = 1;
	/*
	int in2 = GPIO_PIN_IN1;
	if(pin_in == GPIO_PIN_IN1)
	{
		in2 = GPIO_PIN_IN2;
	}
	while(!nrf_gpio_pin_read(in2))
	{
		bool result = nrf_drv_gpiote_in_is_set(pin_in);
	}
	*/
}

void pir_test_call2(nrf_drv_gpiote_pin_t pin_in, nrf_gpiote_polarity_t action)
{
	if(counter == 1)
		counter = 3;
	else if(counter == 0)
		counter = 2;
	return;
}
int main(void)
{
	ParallaxPIR sensor1;
	sensor1.gpio_pin = GPIO_PIN_IN1;
	sensor1.callback_function = pir_test_call;

	ParallaxPIR sensor2;
	sensor2.gpio_pin = GPIO_PIN_IN2;
	sensor2.callback_function = pir_test_call2;
	
	pir_init(&sensor1);
	pir_init(&sensor2);
	int i = 0;
	while(1)
	{
		while(run)
		{
		while(run)
		{
		}
		i++;
		}
	}
	return 0;
}
