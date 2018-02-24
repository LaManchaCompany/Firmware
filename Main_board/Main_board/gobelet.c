
# define F_CPU 16000000UL

#include <avr/io.h>
#include "Bours_scheduler.h" // utilisation de la variable "stack" pour le Bours_scheduler

#include "gobelet.h"
#include "util/delay.h"


#define GobeletPort		PORTE
#define Sensor1		PIN6_bm //6
#define Sensor2		PIN5_bm //5
#define Sensor3		PIN4_bm //4

/* Variable Stack pour le Bours_scheduler */
Stack_function_t Stack_gobelet; 
P_Stack_function_t P_Stack_gobelet = &Stack_gobelet;


init_gobelet()
{

	//	OUT/IN			   76543210
	GobeletPort.DIRSET = 0b10000000;
	//	OUT/IN			   76543210
	GobeletPort.DIRCLR = 0b01110000;

	//	OUT/IN			   76543210
	GobeletPort.OUTSET = 0b10000000;
}


trig_gobelet()
{
	//	OUT/IN			   76543210
	GobeletPort.OUTSET = 0b10000000;
	_delay_ms(400);
	//	OUT/IN			   76543210
	GobeletPort.OUTCLR = 0b10000000;
}


char read_gobelet_sensor_1()
{
	//PORTE 6
	if(GobeletPort.IN & Sensor1) // read input
	{
		return 1;
	}
	else
	{
		return 0;
		
	}
	
}

char read_gobelet_sensor_2()
{
	//PORTE 5
	if(GobeletPort.IN & Sensor2) // read input
	{
		return 1;
	}
	else
	{
		return 0;
		
	}
	
}

char read_gobelet_sensor_3()
{
	//PORTE 4
	if(GobeletPort.IN & Sensor3) // read input
	{
		return 1;
	}
	else
	{
		return 0;
		
	}
	
}

// GIVE ME A GOB
uint8_t give_me_a_gob( P_Stack_function_t P_Stack_gobelet ) 
{
	
	
	uint8_t timer_gob =0;
	uint8_t try_gob_count = 0;
	uint8_t error_gob= 1;
			// SUCCESS			0
			// IN_PROGRESS		1
			// ERROR			2
			//
	uint8_t state_gob =0;
	
					
/*	
	while(try_gob_count < max_try+1)
	{
		if(try_gob_count == max_try)
		{
			error_gob = 1;
			break;
		}
		
		switch(state_gob)
		{
			
			
			case 0: //trig
			
			trig_gobelet();
			try_gob_count ++;
			state_gob = 1;
			
			break;
			
			case 1: // wait falling edge
			
			timer_gob = 0;
			state_gob = 2; // wait rising edge after that
			
			while( read_gobelet_sensor_1()==1  )
			{
			
				if (timer_gob >= time_out)
				{
					
					
					// time out
					state_gob = 0; // try again
					break;
				}
				timer_gob ++;
				_delay_ms(100);
				
			}
			
			
			
			break;
			
			
			
			case 2: // wait rising edge
			
			timer_gob = 0;
			error_gob = 0; // success
			state_gob = 3; // end after that
			
			
			while( read_gobelet_sensor_1()==0  )
			{
				
				if (timer_gob >= time_out)
				{
					// time out
					
					error_gob = 2; // error rising edge time out
					break;
				}
				timer_gob ++;
				_delay_ms(100);
				
			}
			
			
			
			
			break;
			
			case 3: // end
				return error_gob;
			break;
		}
		
		
		
	}
	
	*/
	
}

		