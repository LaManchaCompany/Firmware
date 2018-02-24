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
	PORTA.DIRSET = 0b00001110;
}


void Soft_I2C_init()
{
	Soft_I2C_Writing_condition();
	asm ("nop");asm ("nop");asm ("nop");
	//	OUT/IN		  76543210
	PORTA.OUTSET = 	0b00000011;
	asm ("nop");asm ("nop");asm ("nop");
	PORTA.OUTCLR =  0b00000100;
}



void Soft_I2C_Start()
{
	Soft_I2C_Writing_condition();
	asm ("nop");asm ("nop");asm ("nop");
	//	OUT/IN		 76543210
	PORTA.OUTSET = 0b00000011;
	asm ("nop");asm ("nop");asm ("nop");
	PORTA.OUTCLR = 0b00000001;
}

void Soft_I2C_Stop()
{
	Soft_I2C_Writing_condition();
	asm ("nop");asm ("nop");asm ("nop");
	//	OUT/IN		 76543210
	PORTA.OUTCLR = 0b00000001;
	asm ("nop");asm ("nop");asm ("nop");
	PORTA.OUTSET = 0b00000010;
	asm ("nop");asm ("nop");asm ("nop");
	PORTA.OUTSET = 0b00000001;
	
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
	if (x & m) Soft_I2C_sda_High(); else Soft_I2C_sda_Low();
	Soft_I2C_scl_High();
	asm ("nop");asm ("nop");asm ("nop");
	Soft_I2C_scl_Low();
	asm ("nop");asm ("nop");asm ("nop");
}

static void Soft_I2C_write_uint8(uint8_t x)
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
	 
	
}

void Soft_I2C_write_port()
{
	 uint8_t adress =	0b01111110;
	 uint8_t ext_port = 0b11111111;
	 PORTF.DIRSET = 0b00000011;
	 Soft_I2C_Start();
	 Soft_I2C_write_uint8(adress);
	 
	
	//read ack
	Soft_I2C_Reading_condition();
	 Soft_I2C_scl_High();
	asm ("nop");asm ("nop");asm ("nop");
	Soft_I2C_scl_Low();
	asm ("nop");asm ("nop");asm ("nop");
	
	
	 Soft_I2C_write_uint8(ext_port);
	 
	 //read ack
	 Soft_I2C_Reading_condition();
	 Soft_I2C_scl_High();
	 asm ("nop");asm ("nop");asm ("nop");
	 Soft_I2C_scl_Low();
	 asm ("nop");asm ("nop");asm ("nop");
	 
	 Soft_I2C_Writing_condition();
	 
	 Soft_I2C_Stop();
	 
	 
}