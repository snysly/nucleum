/*
 * Send an advertisement periodically
 */

#include <stdbool.h>
#include <stdint.h>
#include "nrf_gpio.h"
#include "ble_advdata.h"
#include "boards.h"
#include "nordic_common.h"
#include "softdevice_handler.h"
#include "ble_debug_assert_handler.h"
#include "led.h"
#include "nrf_delay.h"

#include "simple_ble.h"
#include "simple_doorjamb_adv.h"
//#include "simple_adv.h"

int main(void) {
    uint32_t err_code;

    led_init(LED_0);
    led_init(LED_1);
    led_init(LED_2);

    // Setup BLE
    simple_adv_init(&ble_config);

    // Advertise because why not
    simple_adv_start();
    uint32_t height = 0;
    action_type_t action = INVALID;
    int i = 0;

    led_on(LED_0);
    while(1)
    {
        nrf_delay_us(1000000);
        //for(int i = 0; i < 5000000; i++) { }
        height += 1000;
        i++;
        action++;
        simple_adv_transaction(height, action);
        if(i % 3 == 0)
        {
            led_on(LED_0);
            led_off(LED_1);
            led_off(LED_2);
        }
        else if(i % 3 == 1)
        {
            led_off(LED_0);
            led_on(LED_1);
            led_off(LED_2);
        }
        else
        {
            led_off(LED_0);
            led_off(LED_1);
            led_on(LED_2);
        }
    }
    /*while (1) {
        power_manage();
    }*/
}
