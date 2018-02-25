/*
 * Bours_pump.h
 *
 * Created: 25-02-18 09:49:44
 *  Author: Jeremie
 */ 


#ifndef BOURS_PUMP_H_
#define BOURS_PUMP_H_

// possibles internal states for give_me_a_gob task
#define INIT_STATE					0
#define WAIT_STATE					1
#define REFRESH_STATE				2
#define FINISH_STATE				3

uint8_t pumps_refresh(uint8_t pump_number);

uint8_t pumps_on_off(P_Stack_function_t P_Stack); // non-blocking Bours-schedulable task


#endif /* BOURS_PUMP_H_ */