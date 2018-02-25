/*
 * serial_communication.c
 *
 * Created: 08-04-17 21:18:14
 *  Author: Jérémie
 */ 
#include "serial_communication.h"
#include "util/delay.h"
#include <asf.h>







struct Command get_next_command(char* command_code,uint8_t* data, uint8_t time_out)
{

	struct Command comm;
	comm.data_lenght =0;
	//comm.datas =0;
	comm.code = 0; 

	uint8_t timer = 0;
	uint8_t error_serial_communication= 0;
		//  0: sucess
		//  1: no data
		//  2: error end of frame time out
	
	uint8_t cdc_number_byte_received =0;
	uint8_t* vect = (uint8_t*) malloc(256);
	uint8_t *cdc_buf =  (uint8_t*) malloc(256);
	uint8_t vect_lenght = 0;


	while(is_eot_present(vect, vect_lenght) != 1) //check the end of a tram (terminated by 3 char "eot"), while is not the case, we have to wait for new bytes...
	{
		_delay_ms(100);
		
		// data available
		if(udi_cdc_is_rx_ready() != 0) // we have received datas
		{
			
			cdc_number_byte_received = udi_cdc_get_nb_received_data();	// how many byte have we received
			udi_cdc_read_buf(cdc_buf, cdc_number_byte_received);		// read the cdc-uart buffer 
			concat_string(vect_lenght,cdc_buf,cdc_number_byte_received, vect);	// concatenate every bytes into a vector
			vect_lenght = vect_lenght + cdc_number_byte_received;							// bytes count 
			
			
			//itoa(offset,chars,10);
			//udi_cdc_write_buf (chars, 2);
			
		}
		else
		{
			//  1: no data
			error_serial_communication = 1;
		}
		
	}
	
	concat_string(0,vect,4,command_code);
	
	free(vect);
	free(cdc_buf);
	return comm;
}


void concat_string(uint8_t offset, const uint8_t *data_in,uint8_t lenght, uint8_t *vector_out)
{
	for(uint8_t i=0; i< lenght ;i++ )
	{
		
		vector_out[i+offset] = data_in[i];
		//vector_out++;
		//udi_cdc_putc(data_in[i]);
	}	
}




char compare_string(const uint8_t *const_str, uint8_t *str_to_be_compared, uint8_t lenght, uint8_t offset)
{
	char flag = 0;
	

	
	for(uint8_t i=0; i < lenght ;i++ )
	{
		
		if (const_str[i] != str_to_be_compared[i+offset] && str_to_be_compared[i+offset] !=0)
		{
			return 0; //fail, one of the chars is different
			
			
		}
		else if (const_str[i] == str_to_be_compared[i+offset] && str_to_be_compared[i+offset] !=0)
		{
			flag = 1; 
		
			
		}
	}
	

	return flag;
}

char is_eot_present(uint8_t *str_to_be_checked, uint8_t lenght)
{
	uint8_t offset = 0;
	if(lenght < 3) {offset =0;}
	else{offset = lenght -3;}
		
	return compare_string("eot",str_to_be_checked,3,offset);
}