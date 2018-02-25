
# define F_CPU 16000000UL

#include <avr/io.h>
#include "Bours_scheduler.h" // utilisation de la variable "stack" pour le Bours_scheduler

#include "gobelet.h"
#include "util/delay.h"


#define GobeletPort		PORTE
#define Sensor1		PIN6_bm //6
#define Sensor2		PIN5_bm //5
#define Sensor3		PIN4_bm //4





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

// return:
// BOURS_TASK_SUCCESS		0
// BOURS_TASK_IN_PROGRESS	1
// BOURS_TASK_DISABLED		2
// BOURS_TASK_ERROR			3
uint8_t give_me_a_gob( P_Stack_function_t P_Stack ) 
{
	if(P_Stack->flag == 0) /* Task disabled */
	{
		return BOURS_TASK_DISABLED;	// return with BOURS_TASK_DISABLED code
	}
	else if(P_Stack->flag == 1) /* Task enabled */
	{
		if(P_Stack->current_try <= P_Stack->max_try +1) /* Wy maybe havent tried too many time  */
		{
			if(P_Stack->current_try == P_Stack->max_try +1) /* We have tried too many time */
			{	
				P_Stack->flag = 0;			// disable task
				P_Stack->current_fuel = 0;	// consume fuel
				P_Stack->current_try = 0;	// reset try count 
				return BOURS_TASK_ERROR;	// return error code
			}
			
			if(P_Stack->current_fuel == 0) /* We have tried too much time (during one try) */
			{
				P_Stack->task_state = TRIG_STATE;			// trig after that
				P_Stack->current_fuel = P_Stack->init_fuel;	// reset fuel
				return BOURS_TASK_IN_PROGRESS;				// return with BOURS_TASK_IN_PROGRESS code
			}
			
		
			switch(P_Stack->task_state)
			{
				case TRIG_STATE: //trig
					trig_gobelet();
					P_Stack->current_try ++;
					P_Stack->task_state = WAIT_FALLING_EDGE_STATE;
					return BOURS_TASK_IN_PROGRESS;		// return with BOURS_TASK_IN_PROGRESS code
				break;
			
				case WAIT_FALLING_EDGE_STATE: // wait falling edge
	
					if(read_gobelet_sensor_1()==1)
					{
						P_Stack->task_state = WAIT_FALLING_EDGE_STATE; // wait falling edge after that
						P_Stack->current_fuel --;			// consume one fuel
						return BOURS_TASK_IN_PROGRESS;		// return with BOURS_TASK_IN_PROGRESS code
					}
					else if(read_gobelet_sensor_1()==0)
					{
						P_Stack->task_state = WAIT_RISING_EDGE_STATE; // wait rising edge after that
						P_Stack->current_fuel --;			// consume one fuel
						return BOURS_TASK_IN_PROGRESS;		// return with BOURS_TASK_IN_PROGRESS code
					}

				break;
	
				case WAIT_RISING_EDGE_STATE: // wait rising edge
			
					if(read_gobelet_sensor_1()==0)
					{
						P_Stack->task_state = WAIT_RISING_EDGE_STATE; // wait rising edge after that
						P_Stack->current_fuel --;			// consume one fuel
						return BOURS_TASK_IN_PROGRESS;		// return with BOURS_TASK_IN_PROGRESS code
					}
					else if(read_gobelet_sensor_1()==1)
					{
						// Success ! 
						P_Stack->flag = 0;			// disable task
						P_Stack->current_fuel = 0;	// consume fuel
						P_Stack->current_try = 0;	// reset try count
						return BOURS_TASK_SUCCESS;	// return with BOURS_TASK_SUCCESS code
					}
				break;

			}
		
		
		
		}
	
		
	
		
		
	}
					

}

		