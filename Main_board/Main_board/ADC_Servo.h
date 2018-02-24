/*
 * ADC.h
 *
 * Created: 27-09-16 15:51:18
 *  Author: Jérémie
 */ 


#include <asf.h>
#include "avr/io.h"

#ifndef ADC_H_
#define ADC_H_


			#define MY_ADC    ADCB
			#define MY_ADC_CH ADC_CH0
			
			#define MY_SERVO_HYST 200
			
			/*
			
			RJ45	Servo	Main Board

			1		In1 	PB0
			2		In2		PB1
			3		En		PB2
			
			*/
			
			
				uint16_t adc_read_16();
				void Servo_init();
				uint16_t align_servo(uint16_t x);
				
			
#endif /* ADC_H_ */