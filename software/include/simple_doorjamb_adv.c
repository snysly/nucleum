/*
 * Advertise just a device name.
 */

// Standard Libraries
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// Nordic Libraries
#include "nordic_common.h"
#include "softdevice_handler.h"
#include "nrf.h"
#include "nrf_sdm.h"
#include "ble.h"
#include "ble_db_discovery.h"
#include "app_util.h"
#include "app_error.h"
#include "ble_advdata.h"
#include "ble_conn_params.h"
#include "ble_hci.h"
#include "app_trace.h"
#include "ble_hrs_c.h"
#include "ble_bas_c.h"
#include "app_util.h"

// Platform, Peripherals, Devices, Services
#include "simple_ble.h"
#include "simple_adv.h"
#include "simple_doorjamb_adv.h"

typedef enum {INVALID, A_B, B_A, A_A, B_B} action_type_t; 

static ble_advdata_manuf_data_t adv_manuf_data;
static ble_advdata_t advdata;
uint8_array_t adv_manuf_data_array;
// bytes 0-3:   height
// byte 4:      action_type
// byte 5:      transaction_count
uint8_t adv_manuf_data_data[6];

void simple_adv_init(simple_ble_config_t * ble_config)
{
    simple_ble_init(ble_config);
}

void simple_adv_start()
{
    uint32_t      err_code;
    //ble_advdata_t advdata;

    // Build and set advertising data
    memset(&advdata, 0, sizeof(advdata));

    advdata.name_type          = BLE_ADVDATA_FULL_NAME;
    advdata.include_appearance = true;
    advdata.flags              = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;

    adv_manuf_data_data[0] = 0;
    adv_manuf_data_data[1] = 0;
    adv_manuf_data_data[2] = 0;
    adv_manuf_data_data[3] = 0;
    adv_manuf_data_data[4] = 0;
    adv_manuf_data_data[5] = 0;

    adv_manuf_data_array.p_data = adv_manuf_data_data;
    adv_manuf_data_array.size = sizeof(adv_manuf_data_data);

    adv_manuf_data.company_identifier = 0xFF00;
    adv_manuf_data.data = adv_manuf_data_array;

    advdata.p_manuf_specific_data = &adv_manuf_data;

    err_code = ble_advdata_set(&advdata, NULL);
    APP_ERROR_CHECK(err_code);

    // Start the advertisement
    advertising_start();

}

uint32_t little_to_big_endianness(uint32_t x)
{
    // Original: 10 32 54 76
    // Desired: 76 54 32 10
    return ((x>>24)&0xff) |   // move byte 3 to byte 0
        ((x<<8)&0xff0000) |   // move byte 1 to byte 2
        ((x>>8)&0xff00) |     // move byte 2 to byte 1
        ((x<<24)&0xff000000); // byte 0 to byte 3
}

void simple_adv_transaction(uint32_t height, action_type_t action)
{
    // copy height into manufacturer specific data
    uint32_t flipped_height = little_to_big_endianness(height);
    memcpy(&adv_manuf_data_data, &flipped_height, sizeof(flipped_height));
    // copy action into memory
    
    memcpy(&adv_manuf_data_data[4], &action, sizeof(action));
    // increase transaction count
    adv_manuf_data_data[5]++;

    // stop advertising
    advertising_stop();
    // reset advertisements
    ble_advdata_set(&advdata, NULL);
    // begin re-advertising
    advertising_start();
}

/*void simple_adv_only_name () {
    uint32_t      err_code;
    ble_advdata_t advdata;

    // Build and set advertising data
    memset(&advdata, 0, sizeof(advdata));

    advdata.name_type          = BLE_ADVDATA_FULL_NAME;
    advdata.include_appearance = true;
    advdata.flags              = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;

    err_code = ble_advdata_set(&advdata, NULL);
    APP_ERROR_CHECK(err_code);

    // Start the advertisement
    advertising_start();
}

void simple_adv_service (ble_uuid_t* service_uuid) {
    uint32_t      err_code;
    ble_advdata_t advdata;
    ble_advdata_t srdata;

    // Build and set advertising data
    memset(&advdata, 0, sizeof(advdata));
    memset(&srdata, 0, sizeof(srdata));

    advdata.name_type               = BLE_ADVDATA_NO_NAME;
    advdata.include_appearance      = true;
    advdata.flags                   = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;
    advdata.uuids_complete.uuid_cnt = 1;
    advdata.uuids_complete.p_uuids  = service_uuid;

    // Put the name in the SCAN RESPONSE data
    srdata.name_type                = BLE_ADVDATA_FULL_NAME;

    err_code = ble_advdata_set(&advdata, &srdata);
    APP_ERROR_CHECK(err_code);

    // Start the advertisement
    advertising_start();
}*/