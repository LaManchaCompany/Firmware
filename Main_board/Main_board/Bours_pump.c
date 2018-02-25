/*
 * Bours_pump.c
 *
 * Created: 25-02-18 09:49:58
 *  Author: Jeremie
 */ 
#include <avr/io.h>
#include "Bours_scheduler.h" // utilisation de la variable "stack" pour le Bours_scheduler
#include "Bours_pump.h"
#include "util/delay.h"
#include "Soft_I2C.h"


// send 8 bit to the pump board, each bit is a pump (1: pump on , 0: pump off)
uint8_t pumps_refresh(uint8_t pumps_mask)
{
	Soft_I2C_write_port(pumps_mask); // nécessite Soft_I2C_init()
	return 0;	
}

//pumps_on_off
// non-blocking Bours-schedulable task

// return:
// BOURS_TASK_SUCCESS		0
// BOURS_TASK_IN_PROGRESS	1
// BOURS_TASK_DISABLED		2
// BOURS_TASK_ERROR			3

// possibles internal states for pumps_on_off task
// INIT_STATE					0
// WAIT_STATE					1
// REFRESH_STATE				2
// FINISH_STATE					3

// P_Stack->variable1 = pump binary mask
// P_Stack->data_table[0 -> 7] pumps on-times
// P_Stack->flag = task enabled or not

uint8_t pumps_on_off(P_Stack_function_t P_Stack) 
{
		if(P_Stack->flag == 0) /* Task disabled */
		{
			return BOURS_TASK_DISABLED;	// return with BOURS_TASK_DISABLED code
		}
		else if(P_Stack->flag == 1) /* Task enabled */
		{
			
				switch(P_Stack->task_state)
				{
					case INIT_STATE:
						
						for(uint8_t i =0; i<7; i++)
						{
							if(P_Stack->data_table[i]	> 1)
							{
								// set to 1
								P_Stack->variable1 |= (1 << i);
							}
							
						}
						
						P_Stack->task_state = REFRESH_STATE;
						return BOURS_TASK_IN_PROGRESS;
						
					break;
					
					case WAIT_STATE:

						for(uint8_t i =0; i<7; i++)
						{
							if(P_Stack->data_table[i]	> 1)
							{	
								// set to 1
								P_Stack->variable1 |= (1 << i);
								P_Stack->data_table[i]--;
							}
							else if(P_Stack->data_table[i]	== 1)	
							{	
								P_Stack->task_state = REFRESH_STATE;
								// Set to zero
								P_Stack->variable1 &= ~(1 << i);
								P_Stack->data_table[i]--;
							}
							else if(P_Stack->data_table[i]	== 0)
							{
								// nothing
							}
						}	
					
						return BOURS_TASK_IN_PROGRESS;
		
					break;
					
					case REFRESH_STATE:
					
						P_Stack->task_state = FINISH_STATE;
					
						for(uint8_t i =0; i<7; i++)
						{
							if(P_Stack->data_table[i] != 0) P_Stack->task_state = WAIT_STATE;
		
						}
					
						pumps_refresh(0xFF);
						//pumps_refresh(P_Stack->variable1);
						return BOURS_TASK_IN_PROGRESS;
					
					break;
					
					case FINISH_STATE:
					
					P_Stack->flag = 0;
					return BOURS_TASK_SUCCESS;
					
					break;
				}
			}
	
	return 0;
	
}