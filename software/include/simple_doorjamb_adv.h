#ifndef __SIMPLE_ADV_H
#define __SIMPLE_ADV_H

// ACTION_TYPES:
// 0x00 = invalid transaction
// 0x01 = through door A->B
// 0x02 = through door other direction B->A
// 0x03 = stuck on side A
// 0x04 = stuck on side B
// returns true on success
typedef enum {INVALID, A_B, B_A, A_A, B_B} action_type_t; 

// Functions
// initializes BLE hardware
void simple_adv_init(simple_ble_config_t * ble_config);
// begins broadcasting
void simple_adv_start();
// Call to transmit each transaction
void simple_adv_transaction(uint32_t height, action_type_t action);

#endif