#ifndef BLE_DOORJAMB_H
#define BLE_DOORJAMB_H

static uint16_t doorjamb_service_handle = 0;

// ACTION_TYPES:
// 0x00 = invalid transaction
// 0x01 = through door A->B
// 0x02 = through door other direction B->A
// 0x03 = stuck on side A
// 0x04 = stuck on side B
// returns true on success
typedef enum {INVALID, A_B, B_A, A_A, B_B} action_type_t; 

void simple_ble_doorjamb_init(uint8_t door_id);
void start_advertising();
bool broadcast_action(float height, uint32_t time_ticks, action_type_t action);

#endif