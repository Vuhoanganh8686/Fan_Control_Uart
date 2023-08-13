/*
 * message_handle.h
 *
 *  Created on: Aug 11, 2023
 *      Author: Acer
 */

#ifndef INC_MESSAGE_HANDLE_H_
#define INC_MESSAGE_HANDLE_H_

#include "stdint.h"

// Define IDs for different frames
#define FRAME_TURN_ON_OFF       0x01
#define FRAME_SELECT_MODE       0x02
#define FRAME_SELECT_FAN_SPEED  0x03
#define FRAME_READ_STATUS       0x04

typedef enum{
	OFF = 0,
	ON = 1
} POWER;
	
typedef enum{
	LEVEL_0 = 0,
	LEVEL_1 = 1,
	LEVEL_2 = 2
} WIND_MODE;
	
typedef enum{
	AUTOMATIC = 0,
	MANUAL = 1
}  CONTROL_MODE;


extern POWER power;
extern WIND_MODE wind_mode;
extern CONTROL_MODE control_mode;
extern int volatile temperature;

// Function to handle received message frames
uint8_t handle_message(unsigned char* received_buffer, unsigned char* sent_buffer);
void process_power_message(unsigned char* received_buffer, unsigned char* sent_buffer);
void process_control_mode_message(unsigned char* received_buffer, unsigned char* sent_buffer);
void process_wind_mode_message(unsigned char* received_buffer, unsigned char* sent_buffer);
uint8_t process_states_request_message(unsigned char* received_buffer, unsigned char* sent_buffer);
uint8_t check_message_data_length(unsigned char* received_buffer, uint8_t expected_value);



#endif /* INC_MESSAGE_HANDLE_H_ */
