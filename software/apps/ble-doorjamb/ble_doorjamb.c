#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "nrf_gpio.h"
#include "ble_advdata.h"
#include "boards.h"
#include "nordic_common.h"
#include "softdevice_handler.h"
#include "ble_debug_assert_handler.h"
#include "led.h"

#include "simple_ble.h"
#include "simple_adv.h"
#include "ble_doorjamb.h"

#define DOORJAMB_SHORT_UUID 0x3210
#define DOOR_ID_SHORT_UUID 0x0123
#define HEIGHT_SHORT_UUID 0x4567
#define TIME_SHORT_UUID 0x89AB
#define TIME_ACTION_SHORT_UUID 0xCDEF
#define ACTION_SHORT_UUID 0x12EF
#define TRANSACTION_SHORT_UUID 0x34CD

static uint8_t door_id = 0x00;
static uint8_t height_value[] = {0x00, 0x00, 0x00, 0x55}; // float
static uint8_t time_original_value[] = {0x00, 0x00, 0x00, 0x05}; //uint32_t
static uint8_t time_action_value[] = {0x00, 0x00, 0x01, 0x00}; //uint32_t
static uint8_t action_value = 0x1; //uint8_t
static uint8_t transaction_value[] = {0x00, 0x10, 0x00, 0x00}; //uint32_t

/* CONFIGURATION: Intervals for advertising and connections */
static simple_ble_config_t ble_config = {
    .platform_id       = 0x80,              // used as 4th octect in device BLE address
    .device_id         = DEVICE_ID_DEFAULT,
    .adv_name          = DEVICE_NAME,       // used in advertisements if there is room
    .adv_interval      = MSEC_TO_UNITS(100, UNIT_0_625_MS),
    .min_conn_interval = MSEC_TO_UNITS(500, UNIT_1_25_MS),
    .max_conn_interval = MSEC_TO_UNITS(1000, UNIT_1_25_MS)
};

// Here are some more UIUDs for whatever reason
ble_uuid_t service_detailed_uuid = {
    .uuid = (uint16_t)0xFFFE,
    .type = 0x01//BLE_UIUD_TYPE_UNKNOWN
};

// Create Doorjamb service with an unofficial 128-bit UIUD
const ble_uuid128_t service_uiud = {
    {0xEF, 0xCD, 0xAB, 0x89, 0x67, 0x45, 0x23, 0x01, 
    0xEF, 0xCD, 0xAB, 0x89, 0x67, 0x45, 0x23, 0x01}
};

/* CHARACTERISTIC STRUCTURES */
// door id characteristic
ble_gatts_char_handles_t door_id_attr_handle = {
    .value_handle = 0x109
};

// height characteristic
ble_gatts_char_handles_t height_attr_handle = {
    .value_handle = 0x10A
};

// original timestamp characteristic
ble_gatts_char_handles_t time_attr_handle = {
    .value_handle = 0x10B
};

// action timestamp characteristic (at boot)
ble_gatts_char_handles_t action_time_attr_handle = {
    .value_handle = 0x10C
};

// action/direction characteristic (at action)
ble_gatts_char_handles_t action_attr_handle = {
    .value_handle = 0x10D
};


// transaction id characteristic (at action)
ble_gatts_char_handles_t transaction_attr_handle = {
    .value_handle = 0x10E
};

// returns doorjamb service handle
uint16_t create_doorjamb_service()
{
    doorjamb_service_handle = simple_ble_add_service(&service_uiud,
        &service_detailed_uuid, DOORJAMB_SHORT_UUID);

    // Create doorjamb door id characteristic
    simple_ble_add_characteristic(
        1, 0, 1, service_detailed_uuid.type, DOOR_ID_SHORT_UUID, sizeof(door_id), 
        &door_id, doorjamb_service_handle, &door_id_attr_handle
    );

    // Create doorjamb height characteristic
    simple_ble_add_characteristic(
        1, 0, 1, service_detailed_uuid.type, HEIGHT_SHORT_UUID, sizeof(height_value), 
        height_value, doorjamb_service_handle, &height_attr_handle
    );

    // Create doorjamb original timestamp characteristic (at boot)
    simple_ble_add_characteristic(
        1,0,1,service_detailed_uuid.type, TIME_SHORT_UUID, sizeof(time_original_value),
        time_original_value, doorjamb_service_handle, &time_attr_handle
    );

    // Create doorjamb action timestamp characteristic (at action)
    simple_ble_add_characteristic(
        1,0,1,service_detailed_uuid.type, TIME_ACTION_SHORT_UUID, sizeof(time_action_value),
        time_action_value, doorjamb_service_handle, &action_time_attr_handle
    );

    // Create doorjamb action/direction characteristic
    simple_ble_add_characteristic(
        1,0,1,service_detailed_uuid.type, ACTION_SHORT_UUID, sizeof(action_value),
        &action_value, doorjamb_service_handle, &action_attr_handle
    );

    // Create doorjamb transaction id characteristic
    simple_ble_add_characteristic(
        1,0,1,service_detailed_uuid.type, TRANSACTION_SHORT_UUID, sizeof(transaction_value),
        transaction_value, doorjamb_service_handle, &transaction_attr_handle
    );

    return doorjamb_service_handle;
}

// Initialize BLE and Doorjamb Service
void simple_ble_doorjamb_init(uint8_t door_id_param)
{
    door_id = door_id_param;
    // initialize BLE
    simple_ble_init(&ble_config);

    // initialize doorjamb service
    doorjamb_service_handle = create_doorjamb_service();
}

// Start and manage advertising
void start_advertising()
{
    simple_adv_service(&service_detailed_uuid);
}

// Handles request to broadcast new actions
bool broadcast_action(float height, uint32_t time_ticks, 
    action_type_t action)
{
    // overwrite height, action_time, action_type fields
    memcpy(height_value, &height, sizeof(height));
    memcpy(time_action_value, &time_ticks, sizeof(time_ticks));
    memcpy(&action_value, &action, sizeof(action));
    // may need to implement swapping from little to big endian at one end
    // of the BLE transfer - here's the GCC implementation, no libraries needed
    //__builtin_bswap32(&(uint32_t *)time_ticks);
    
    // increment height field
    uint32_t transaction = 0;
    memcpy(&transaction, transaction_value, sizeof(transaction));
    transaction += 1;
    memcpy(transaction_value, &transaction, sizeof(transaction));

    // new data will be advertised with next new advertisement
    return 0;
}