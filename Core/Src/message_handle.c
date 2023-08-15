/*
 * message_handle.c
 *
 *  Created on: Aug 11, 2023
 *      Author: Acer
 */
#include "flash.h"
#include "message_handle.h"
#include "stdint.h"
#include <stdio.h>


POWER power = OFF;
WIND_MODE wind_mode = LEVEL_0;
CONTROL_MODE control_mode = AUTOMATIC;


uint8_t handle_message(unsigned char* received_buffer, unsigned char* sent_buffer) {

    *sent_buffer = 0x02;
    uint8_t send_length;
    uint8_t* stx = received_buffer;
    if(*stx != 0x02){
        send_length = 0;
        goto error_msg;
    }

    uint8_t* txRxID  = received_buffer + 1;
	uint8_t* sent_frame_type = sent_buffer + 1;
	uint8_t* p_sent_data_length = sent_buffer + 2;
	uint8_t sent_data_length;
	uint8_t received_data_length;
    
    switch (*txRxID) {
        case FRAME_TURN_ON_OFF:
            process_power_message(received_buffer, sent_buffer);
            received_data_length = 1;
            sent_data_length = 1;
            break;
        
        case FRAME_SELECT_MODE:
            process_control_mode_message(received_buffer, sent_buffer);
            received_data_length = 1;
            sent_data_length = 1;
            break;
        
        case FRAME_SELECT_FAN_SPEED:
            process_wind_mode_message(received_buffer, sent_buffer);
            received_data_length = 1;
            sent_data_length = 1;
            break;
        
        case FRAME_READ_STATUS:
            return process_states_request_message(received_buffer, sent_buffer);
            received_data_length = 0;
            sent_data_length =  process_states_request_message(received_buffer, sent_buffer);
        
        default:
            send_length = 0;
            goto error_msg;
    }

    *sent_frame_type = *txRxID+ 0x80;
	*p_sent_data_length = sent_data_length;

    uint8_t* received_ETX = received_buffer + 3 + received_data_length;
	if(*received_ETX != 0x03){
		send_length = 0;
		goto error_msg;
    }

    uint8_t* sent_ETX = sent_buffer + 3 + sent_data_length; 
	*sent_ETX = 0x03;
	send_length = sent_data_length + 4;

    error_msg : return send_length;
}

void process_power_message(unsigned char* received_buffer, unsigned char* sent_buffer) {
    if (check_message_data_length(received_buffer, 1)) {
    uint8_t* received_data = received_buffer + 3;
	uint8_t* sent_data = sent_buffer + 3;
	if(*received_data == OFF){
		mutate_power(OFF);
		*sent_data = 0x00;
	}
	else if(*received_data == ON){
		mutate_power(ON);
		*sent_data = 0x00;
	}
	else{
		*sent_data = 0xFF;
	}
    } 
}

void process_control_mode_message(unsigned char* received_buffer, unsigned char* sent_buffer) {
    // Implement logic to process control mode message
    // Update control_mode and sent_buf accordingly
    if (check_message_data_length(received_buffer, 1)){
    uint8_t* received_data = received_buffer + 3;
	uint8_t* sent_data = sent_buffer + 3;
	if(get_power() == OFF){
		*sent_data = 0xFF;
		goto error_msg;
	}
	if(*received_data == AUTOMATIC || *received_data == MANUAL){
		mutate_control_mode(*received_data);
		*sent_data = 0x00;
	}
	else{
		*sent_data = 0xFF;
	}
} goto error_msg;

error_msg: return;

}

void process_wind_mode_message(unsigned char* received_buffer, unsigned char* sent_buffer) {
    // Implement logic to process wind mode message
    // Update wind_mode and sent_buf accordingly
    if (check_message_data_length(received_buffer, 1))
    {
    uint8_t* received_data = received_buffer + 3;
	uint8_t* sent_data = sent_buffer + 3;
	if(get_power() == OFF){
		*sent_data = 0xFF;
		goto error_msg;
	}

	if(*received_data == LEVEL_0 || *received_data == LEVEL_1 || *received_data == LEVEL_2){
		mutate_wind_mode(*received_data);
		*sent_data = 0x00;
		mutate_control_mode(MANUAL);
	}
	else{
		*sent_data = 0xFF;
	}
	error_msg: return;
    }

}

uint8_t process_states_request_message(unsigned char* received_buffer, unsigned char* sent_buffer) {
    // Implement logic to process states request message
    // Update sent_buf accordingly
    uint8_t* sent_data = sent_buffer + 3;
	uint8_t* power_state = sent_data + 1;
	uint8_t* control_mode_state = sent_data + 2;
	uint8_t* wind_mode_state = sent_data + 3;	
	uint8_t* temperature_state = sent_data + 4;
	uint8_t sent_data_length;
	if(!check_message_data_length(received_buffer, 0)){
		sent_data_length = 1;
		*sent_data = 0xFF;
	}
	else{
		*sent_data = 0x00;
		*power_state = get_power();
		*control_mode_state = get_control_mode();
		*wind_mode_state = get_wind_mode();
		int temp = (int) temperature;
		if(temp < 0){
			temp = 0;
		}
		*temperature_state = (uint8_t) temp;
		sent_data_length = 5;
	}
	return sent_data_length;
}

uint8_t check_message_data_length(unsigned char* received_buffer, uint8_t expected_value) {
    uint8_t* length = received_buffer + 2;
    if (*length == expected_value) {
        return 1; // Data length is correct
    } else {
        return 0; // Data length is incorrect
    }
}



