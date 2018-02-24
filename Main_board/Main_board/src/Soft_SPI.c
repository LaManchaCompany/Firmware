/*
 * Soft_SPI.c
 *
 * Created: 10-09-16 11:28:51
 *  Author: Jérémie
 */ 




#include "Soft_SPI.h"


//PA0 -> SS
//PA1 -> MOSI
//PA2 -> MISO
//PA3 -> SCK

#define _BV(bit)  (1 << (bit))


void wait_SPI()
{
	for(int i =0;i<500;i++)
	{
		asm ("nop");asm ("nop");asm ("nop");
		asm ("nop");asm ("nop");asm ("nop");
		asm ("nop");asm ("nop");asm ("nop");
		asm ("nop");asm ("nop");asm ("nop");
		asm ("nop");asm ("nop");asm ("nop");
		asm ("nop");asm ("nop");asm ("nop");
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
uint8_t SPIDir = 0 ;


   #define SPIPort PORTC
   #define SPIMO   5 //5
   #define SPIMI   6
   #define SPISC   7
   #define SPISS   4
   
void hard_spi_init()
{
	
			//sysclk_enable_module(SYSCLK_PORT_C, SYSCLK_SPI);
			//sysclk_enable_module(SYSCLK_PORT_C,PR_SPI_bm);
			sysclk_enable_peripheral_clock(&SPIC);
	 
	 PORTC.DIRSET = PIN4_bm | PIN5_bm | PIN7_bm;
	 PORTC.OUTSET = PIN4_bm | PIN5_bm | PIN7_bm;
	 PORTC.DIRCLR = PIN6_bm; //set MISO input
	// SPIC.CTRL = SPI_ENABLE_bm  |SPI_MODE_0_gc | SPI_MASTER_bm | SPI_PRESCALER_DIV128_gc;
		 SPIC.CTRL = 		SPI_MODE_0_gc|SPI_ENABLE_bm|SPI_PRESCALER_DIV128_gc|SPI_MASTER_bm;
	//SPIC.INTCTRL = 0x03; // assign high priority to SPI interrupt
	//PORTC.OUTSET = 0xff;

}


void spiclose(void)
{
	SPIC.CTRL = 0;
	SPIPort.DIR = SPIDir ;
	//SPIPort.DIRSET = (1<<SPIMI);
}


uint8_t spi(uint8_t val)
{
						//PORTC.OUTCLR = 0b00010000;
						//spi(0x33);
						//PORTC.OUTSET = 0b00010000;
						
	
		
	
		SPIC.DATA = val ;
		while(!(SPIC.STATUS & SPI_IF_bm));
	
	

	
	return SPIC.DATA;
	
}
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

//
//if( (SPIC.STATUS & SPI_IF_bm))
//{
	//
//}
//if( !(SPIC.STATUS & SPI_IF_bm))
//{
	////	PORTF.OUTCLR = 0b00000001 ; // set the output high.)
//}


