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
// Sends a pre-designated "yes I'm available, not seen a transaction"
void simple_adv_start();
// Call to transmit each transaction
void simple_adv_transaction(uint32_t height, action_type_t action);
//void simple_adv_only_name ();
//void simple_adv_service (ble_uuid_t* service_uuid);

#endif