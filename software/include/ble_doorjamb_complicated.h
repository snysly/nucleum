#ifndef __BLE_DOORJAMB_COMPLICATED_H__
#define __BLE_DOORJAMB_COMPLICATED_H__
/*
 * This is the custom Doorjamb Service, with four key characteristics.
 * 
 * HEIGHT: The reported height of the individual, as a 64-bit float.
 * 		TYPE: 32-bit float
 * ORIGINAL_TIME: The absolute (not relative) time recorded during boot
 * 		TYPE: 32-bit integer
 * ACTION_TIME: The absolute (not relative) time that the action was reported,
 * in seconds
 * 		TYPE: 32-bit integer
 * ACTION: Enum signifying the action that occured
 * 		TYPE: 8-bit integer
 * 			0x00: reporting original time
 * 			0x01: A->B (room A to room B)
 * 			0x02: B->A (room B to room A)
 * 			0x03: A->A
 * 			0x04: B->B
 */

typdef enum {ERROR, a_b, b_a, a_a, b_b} action_type_t; 

/* Action structure - contains one transaction through doorframe */
typedef struct action_type_t
{
	float 			height;
	uint32_t		original_time;
	uint32_t		action_time;
	action_type_t	action;
};

/* Doorjamb Service structure. Contains various status info for the service.*/
typedef struct ble_djb_s
{
	ble_djb_evt_handler_t 			evt_handler;
	float							height;
	uint32_t						original_time;
	uint32_t						action_time;
	action_type_t					action;
	uint16_t						service_handle;
	ble_gatts_char_handles_t		ht_handle;
	ble_gatts_char_handles_t		o_time_handle;
	ble_gatts_char_handles_t		a_time_handle;
	ble_gatts_char_handles_t		action_handle;
} ble_djb_t;

/* Function for initializing the Doorjamb Service 
 * Inputs: 		Doorjamb Service structure
 * 				Doorjamb Service Init structure
 * Returns: 	NRF_SUCCESS or an error code
 */
uint32_t ble_djb_init(ble_djb_t * p_djb, const ble_djb_init_t * p_dbj_init);

/* Function for handling Application's BLE Stack events.
 * Inputs: 		p_djb		Doorjamb Service structure
 *				ble_evt_t 	Event received from BLE stack
 */
void ble_djb_on_ble_evt(ble_djb_t * p_djb, ble_evt_t *p_djb_evt);

/* Function for sending an action, since notification is enabled */
void ble_djb_action_send(ble_djb_t * p_djb, action_type_t * action);
#endif
