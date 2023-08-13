/*
 * message_handle.c
 *
 *  Created on: Aug 11, 2023
 *      Author: Acer
 */
#include "message_handle.h"
#include "stdint.h"


POWER power = OFF;
WIND_MODE wind_mode = LEVEL_0;
CONTROL_MODE control_mode = AUTOMATIC;
int volatile temperature = 0;


uint8_t handle_message(unsigned char* received_buffer, unsigned char* sent_buffer) {
    uint8_t* txRxID = received_buffer[1];
    *sent_buffer = 0x02;
    uint8_t send_length;
    uint8_t* stx =received_buffer;
    if(*stx != 0x02){
        send_length = 0;
        goto error_msg;
    }

    uint8_t* received_frame_type  = received_buffer + 1;
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

    error_msg : printf("Length of buffer sent is : %d", send_length);
    return 0; // No response needed
}

void process_power_message(unsigned char* received_buffer, unsigned char* sent_buffer) {
    if (check_message_data_length(received_buffer, 2)) {
        if (received_buffer[2] == 0) {
            power = OFF;
        } else if (received_buffer[2] == 1) {
            power = ON;
        } else {
            sent_buffer[1] |= 0x80; // Set the MSB to indicate an error response
            sent_buffer[2] = 0x01; // Giá trị điều khiển bị sai
        }
    } else {
        sent_buffer[1] |= 0x80; // Set the MSB to indicate an error response
        sent_buffer[2] = 0xFF; // Frame nhận bị lỗi
    }
}

void process_control_mode_message(unsigned char* received_buffer, unsigned char* sent_buffer) {
    // Implement logic to process control mode message
    // Update control_mode and sent_buf accordingly
}

void process_wind_mode_message(unsigned char* received_buffer, unsigned char* sent_buffer) {
    // Implement logic to process wind mode message
    // Update wind_mode and sent_buf accordingly
}

uint8_t process_states_request_message(unsigned char* received_buffer, unsigned char* sent_buffer) {
    // Implement logic to process states request message
    // Update sent_buf accordingly
    return 0; // Return the response length
}

uint8_t check_message_data_length(unsigned char* received_buffer, uint8_t expected_value) {
    if (received_buffer[3] == expected_value) {
        return 1; // Data length is correct
    } else {
        return 0; // Data length is incorrect
    }
}



