/*
 * serial_communication.h
 *
 * Created: 08-04-17 21:37:04
 *  Author: Jérémie
 */ 


#ifndef SERIAL_COMMUNICATION_H_
#define SERIAL_COMMUNICATION_H_

#include <avr/io.h>



struct Command {
	uint8_t code;
	uint8_t datas[256];
	uint8_t data_lenght;
};



struct Command get_next_command(char* command_code,uint8_t* data, uint8_t time_out);
void concat_string(uint8_t offset, const uint8_t *data_in,uint8_t lenght, uint8_t *vector_out);
char compare_string(const uint8_t *const_str, uint8_t *str_to_be_compared,uint8_t lenght, uint8_t offset);
char is_eot_present(uint8_t *str_to_be_checked, uint8_t lenght);


#endif /* SERIAL_COMMUNICATION_H_ */