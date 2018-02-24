/*
 * Soft_I2C.c
 *
 * Created: 28-08-16 09:12:59
 *  Author: Jérémie
 */

#include "Soft_I2C.h"
#include <avr/io.h>

// PA0 = SDA
// PA1 = SCL
// PA2 = /WC (always low in this case)



void wait()
{
	for(int i =0;i<100;i++)
	{
	 asm ("nop");asm ("nop");asm ("nop");
	 asm ("nop");asm ("nop");asm ("nop");
	 asm ("nop");asm ("nop");asm ("nop");
	 asm ("nop");asm ("nop");asm ("nop");
	 asm ("nop");asm ("nop");asm ("nop");
	 asm ("nop");asm ("nop");asm ("nop");
	}
}

//make sure pins are output
void Soft_I2C_Writing_condition()
{
	//	OUT/IN		 76543210
	PORTA.DIRSET = 0b00001111;
}

//make sure SDa pin is an input
void Soft_I2C_Reading_condition()
{
	//	OUT/IN		 76543210
	PORTA.DIRCLR = 0b00000001;
	
	//PORTA.DIR    =    (1<<);     // by name
}


void Soft_I2C_init()
{
	Soft_I2C_Writing_condition();
	wait();
	//	OUT/IN		  76543210
	PORTA.OUTSET = 	0b00000011;
	wait();
	PORTA.OUTCLR =  0b00000100;
}



void Soft_I2C_Start()
{
	Soft_I2C_Writing_condition();
	wait();
	//	OUT/IN		 76543210
	PORTA.OUTSET = 0b00000011;
	wait();
	PORTA.OUTCLR = 0b00000001;
	wait();
	PORTA.OUTCLR = 0b00000010;
	wait();
}

void Soft_I2C_Stop()
{
	Soft_I2C_Writing_condition();
	Soft_I2C_scl_Low();
	wait();
	
	Soft_I2C_sda_High();

	wait();
    Soft_I2C_scl_High();
	wait();

	
}

//-------------- SCL ---------------//

void Soft_I2C_scl_High()
{
	//	OUT/IN		 76543210
	PORTA.OUTSET = 0b00000010;
}

void Soft_I2C_scl_Low()
{
	//	OUT/IN		 76543210
	PORTA.OUTCLR = 0b00000010;
}
///////////////////////////////////////


//-------------SDA ------------------//
void Soft_I2C_sda_High()
{
	//	OUT/IN		 76543210
	PORTA.OUTSET = 0b00000001;
}

void Soft_I2C_sda_Low()
{
	//	OUT/IN		 76543210
	PORTA.OUTCLR = 0b00000001;
}
///////////////////////////////////////



void Soft_I2C_write_bit(uint8_t x, uint8_t m)
{
	Soft_I2C_Writing_condition();
	if (x & m) Soft_I2C_sda_High(); else Soft_I2C_sda_Low();
	wait();
	Soft_I2C_scl_High();
	wait();
	Soft_I2C_scl_Low();
	wait();
}

static void Soft_I2C_write_adress_uint8(uint8_t x)
{
	
	/* transmit msb first */
	
	  Soft_I2C_write_bit(x, (1 << 7));
	  Soft_I2C_write_bit(x, (1 << 6));
	  Soft_I2C_write_bit(x, (1 << 5));
	  Soft_I2C_write_bit(x, (1 << 4));
	  Soft_I2C_write_bit(x, (1 << 3));
	  Soft_I2C_write_bit(x, (1 << 2));
	  Soft_I2C_write_bit(x, (1 << 1));
	  Soft_I2C_write_bit(x, (1 << 0));
	  
	  
	 
	 	//read ack
		Soft_I2C_sda_High();
		Soft_I2C_Reading_condition();
		Soft_I2C_sda_High();
		wait();
	 	Soft_I2C_scl_High();
	 	wait();
	 	Soft_I2C_scl_Low();
	 	wait();
		 
	
	
}

static void Soft_I2C_write_command_uint8(uint8_t x)
{
	
	/* transmit msb first */
	
	Soft_I2C_write_bit(x, (1 << 7));
	Soft_I2C_write_bit(x, (1 << 6));
	Soft_I2C_write_bit(x, (1 << 5));
	Soft_I2C_write_bit(x, (1 << 4));
	Soft_I2C_write_bit(x, (1 << 3));
	Soft_I2C_write_bit(x, (1 << 2));
	Soft_I2C_write_bit(x, (1 << 1));
	Soft_I2C_write_bit(x, (1 << 0));
	
	
	
	//read ack
	Soft_I2C_sda_High();
	Soft_I2C_Reading_condition();
	Soft_I2C_sda_High();
	wait();
	Soft_I2C_scl_High();
	
	wait();
	
	
	
}


void Soft_I2C_write_port(uint8_t x)
{
	
	// uint8_t adress =	0b01111110;
	 uint8_t adress =	0b01111110;
	 
	 
	 PORTF.DIRSET = 0b00000011;
	 
	 Soft_I2C_Start();
	 
	 Soft_I2C_write_adress_uint8(adress);
	
	 Soft_I2C_write_command_uint8(x);
	 
	 Soft_I2C_Writing_condition();
	 
	 //Soft_I2C_Stop();
	 
	 
}