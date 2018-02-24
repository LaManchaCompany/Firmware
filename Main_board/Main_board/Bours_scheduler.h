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
	La physolosophie du scheduler et de créer des tâches non bloquantes et de les appeler si besoin.
	Si une tâche doit s'exécuter pendant un long moment, au lieu de bloquer l'exécution de la boucle principale
	elle mémorise l'état de son travail dans son stack , puis retourne. Si son travail n'est pas terminé lors du prochain appel 
	elle continue son travail, et ainsi de suite.
	Le programe principal lui alloue du "fuel" pour exécuter son travail. 
	A chaque appel de la fonction, son fuel diminue de une unité, c'est l'équivalent d'un "timeout".
	Ce fuel est une unité de temps, car la boucle principale s'exécute de façon régulière dans le temps. (tick)

	Ceci implique que toute fonctions appelée utilise ce "stack", soit non bloquante, et continue son "travail" à chaque appel
  
  */


#define BOURS_TASK_SUCCESS			0
#define BOURS_TASK_IN_PROGRESS		1
#define BOURS_TASK_ERROR			2



// Stack , variable permettant aux fonctions appelée par le "scheduler" de garder leur état en vie entre chaque appel
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

// Function_caller appel la fonction passée en argument , avec un pointeur vers son "stack"
uint8_t Function_caller(P_Stack_function_t P_stack, uint8_t (*ptrfonction)(P_Stack_function_t));






#endif /* BOURS_SCHEDULER_H_ */