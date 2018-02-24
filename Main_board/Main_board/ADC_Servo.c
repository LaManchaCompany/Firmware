/*
 * ADC.c
 *
 * Created: 27-09-16 15:51:01
 *  Author: Jérémie
 */ 
#include "ADC_Servo.h"

void adc_init()
{
	
		
		struct adc_config adc_conf;
		struct adc_channel_config adcch_conf;
			
		adc_read_configuration(&MY_ADC, &adc_conf);
		adcch_read_configuration(&MY_ADC, MY_ADC_CH, &adcch_conf);
		adc_set_conversion_parameters(&adc_conf, ADC_SIGN_OFF, ADC_RES_12,ADC_REF_BANDGAP);
		adc_set_conversion_trigger(&adc_conf, ADC_TRIG_MANUAL, 1, 0);
		adc_set_clock_rate(&adc_conf, 200000UL);
		adcch_set_input(&adcch_conf, ADCCH_POS_PIN3, ADCCH_NEG_NONE, 1);
		adc_write_configuration(&MY_ADC, &adc_conf);
		adcch_write_configuration(&MY_ADC, MY_ADC_CH, &adcch_conf);
		sysclk_enable_peripheral_clock(&MY_ADC);
		adc_enable(&MY_ADC);
		PORTB.DIRSET = 0xff;
}

uint16_t adc_read_16()
{
				
		adc_start_conversion(&MY_ADC, MY_ADC_CH);
		adc_wait_for_interrupt_flag(&MY_ADC, MY_ADC_CH);
		return adc_get_result(&MY_ADC, MY_ADC_CH);
}

void Servo_init()
{
	 adc_init();
	 PORTB.DIRSET = 0b00000111; // Enable , dir1 , dir2 as output
}

uint16_t align_servo(uint16_t target)
{
			/*
			
			RJ45	Servo	Main Board

			1		In1 	PB0
			2		In2		PB1
			3		En		PB2
			
			*/
	 uint16_t delta = 0;
	 uint16_t position;
	 char flag;
	
	 position = adc_read_16();
	 
	 if(position > target)
	 {
		 delta = position - target;
		 PORTB.OUTCLR = 0b00000010; // dir2 = 0
		 PORTB.OUTSET = 0b00000001; // dir1 = 1
	 }
	 if(position < target)
	 {
		 delta = target - position;
		 PORTB.OUTCLR = 0b00000001; // dir1 = 0
		 PORTB.OUTSET = 0b00000010; // dir0 = 1
	 }
	 
	 if(delta > MY_SERVO_HYST)
	 {
		 PORTB.OUTSET = 0b00000100; // Enable = 1
		 
		 
	 }
	 else
	 {
		  PORTB.OUTCLR = 0b00000100; // Enable = 0
		 
		  
	 }
	 
	 return delta;
	 
	 
}