/*
 * Bours_scheduler.c
 *
 * Created: 24-02-18 13:24:55
 *  Author: Jeremie
 */ 

#include <avr/io.h>
#include "Bours_scheduler.h"


// Function_caller appel la fonction passée en argument , avec un pointeur vers son "stack"
uint8_t Function_caller(P_Stack_function_t P_stack, uint8_t (*ptrfonction)(P_Stack_function_t))
{
	return (*ptrfonction)(P_stack);       /*Appel de la fonction pointée par ptrfonction*/
}