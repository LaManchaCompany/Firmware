/*
 * Bours_scheduler.h
 *
 * Created: 24-02-18 13:24:40
 *  Author: Jeremie
 */ 


#ifndef BOURS_SCHEDULER_H_
#define BOURS_SCHEDULER_H_

#include <avr/io.h>
/* 
	La physolosophie du scheduler et de cr�er des t�ches non bloquantes et de les appeler si besoin.
	Si une t�che doit s'ex�cuter pendant un long moment, au lieu de bloquer l'ex�cution de la boucle principale
	elle m�morise l'�tat de son travail dans son stack , puis retourne. Si son travail n'est pas termin� lors du prochain appel 
	elle continue son travail, et ainsi de suite.
	Le programe principal lui alloue du "fuel" pour ex�cuter son travail. 
	A chaque appel de la fonction, son fuel diminue de une unit�, c'est l'�quivalent d'un "timeout".
	Ce fuel est une unit� de temps, car la boucle principale s'ex�cute de fa�on r�guli�re dans le temps. (tick)

	Ceci implique que toute fonctions appel�e utilise ce "stack", soit non bloquante, et continue son "travail" � chaque appel
  
  */


#define BOURS_TASK_SUCCESS			0
#define BOURS_TASK_IN_PROGRESS		1
#define BOURS_TASK_ERROR			2



// Stack , variable permettant aux fonctions appel�e par le "scheduler" de garder leur �tat en vie entre chaque appel
typedef struct Stack_function
{
	uint8_t flag;	
	uint8_t max_fuel;  						
	uint8_t current_fuel;
	uint8_t max_try;
	uint8_t current_try;
	uint8_t task_state;
	uint8_t variable1;
	uint8_t variable2;
	uint8_t data_table[10];
}Stack_function_t;

/*   P_Stack_function_t type pointeur vers struct Stack_function_t */
typedef Stack_function_t *P_Stack_function_t;   

// Function_caller appel la fonction pass�e en argument , avec un pointeur vers son "stack"
uint8_t Function_caller(P_Stack_function_t P_stack, uint8_t (*ptrfonction)(P_Stack_function_t));






#endif /* BOURS_SCHEDULER_H_ */