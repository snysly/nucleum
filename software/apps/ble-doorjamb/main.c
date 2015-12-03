/*
 * Send an advertisement periodically
 */

#include <stdbool.h>
#include <stdint.h>
#include "nrf_gpio.h"
#include "boards.h"
#include "nordic_common.h"
#include "softdevice_handler.h"
#include "led.h"
#include "ble_advdata.h"
#include "ble_debug_assert_handler.h"
//#include "app_timer.h"

#include "simple_ble.h"
#include "ble_doorjamb.h"

// This should be changed for each door module!
// Software will be set up to chain them, starting at 1...n
#define DOOR_ID 1

int main(void)
{
    //uint32_t err_code;
    led_init(LED_0);
    led_init(LED_1);
    led_init(LED_2);

    // Setup BLE
    simple_ble_doorjamb_init(DOOR_ID);

    // Start advertising
    start_advertising();

    // If LED is white vs. off, we've successfully started advertising
    float test = 5.0;
    uint32_t timer = 5;
    action_type_t experiment = INVALID;
    led_on(LED_0);
    led_on(LED_1);
    led_on(LED_2);
    while (1) {
        //broadcast_action(test,timer, experiment);
        power_manage();
    }
}
