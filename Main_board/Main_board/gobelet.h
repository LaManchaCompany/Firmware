
//gobelet board header

#include <avr/io.h>
#include "Bours_scheduler.h" // utilisation de la variable "stack" pour le Bours_scheduler

// possibles internal states for give_me_a_gob task
#define TRIG_STATE					0
#define WAIT_FALLING_EDGE_STATE		1
#define WAIT_RISING_EDGE_STATE		2

void init_gobelet();
void trig_gobelet();
char read_gobelet_sensor_1();
char read_gobelet_sensor_2();
char read_gobelet_sensor_3();
uint8_t give_me_a_gob(P_Stack_function_t P_Stack); // time out in 0.1 sec