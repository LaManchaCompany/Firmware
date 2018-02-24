/*
 * LT2449.c
 *
 * Created: 11-11-16 19:52:20
 *  Author: Jérémie
 */ 
#include <avr/io.h>
#include "LT2449.h"
#include "compiler.h"

//#define set_bit number |= 1 << x;

   #define SPIPort		PORTC
   #define SPIMO		PIN5_bm//5
   #define SPIMI		PIN6_bm //6
   #define SPIMI_IN		PIN6_bm
   #define SPISC		PIN7_bm //7
   #define SPISS		PIN4_bm //4
   
 /*
 
 
 31	30	29	28	27	26	25	24	23	22	21	20	19	18	17	16	15	14	13	12	11	10	9	8	7	6	5	4	3	2	1	0
 /EOC	DMY (0)	1 si IN > 0 	MSB																							LSB
 0 si end		0 si IN < 0

 */
	
   


void LTC2449_init_port()
{
	   
	   SPIPort.DIRSET = SPISS | SPIMO | SPISC; // set Chip select (slave select) , MOSI and SCK as output
	   SPIPort.OUTSET = SPISS | SPIMO | SPISC; // set them to HIGH 
	   SPIPort.DIRCLR = SPIMI; //set MISO input

}

void wait_SPI_ltc2449()
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


void SPI_togle_sck()
{
	SPIPort.OUTCLR = SPISC;
	wait_SPI_ltc2449();
	SPIPort.OUTSET = SPISC;
	wait_SPI_ltc2449();
}

uint32_t  SPI_bit_bang(uint8_t lenght, uint32_t data_out)
{
	uint8_t test=0;
	//uint32_t mask_in =  0x80000000;
	uint32_t mask_out = 0x10000000;
	uint32_t result = 0; 
	
	SPIPort.OUTCLR = SPISS; //select slave
	wait_SPI_ltc2449();
	//for (uint8_t i=0; i< lenght; i++ )
	for (uint8_t i = 0; i < 32; i++ ) //32 bit bit_bang spi
	{
		// let's bit bang motherfucker
		// 
		if( Tst_bits(data_out,mask_out) ) // write output
		{
			SPIPort.OUTSET = SPIMO;
		}
		else
		{
			SPIPort.OUTCLR = SPIMO;
		}
		
		SPI_togle_sck();
		
		if(	SPIPort.IN & SPIMI	) // read input
		{
			result |= 1 << i; // set bit A VERIFIER (ordre MSB ou LSB) http://stackoverflow.com/questions/47981/how-do-you-set-clear-and-toggle-a-single-bit-in-c-c
			//result ++;
			//Set_bits(result,mask_in);
		}
		else
		{
			//Clr_bits(result,mask_in);
		}
		
		//mask_in		>>	1; // rotate // A VERIFIER
		mask_out	<<	1; // rotate // A VERIFIER
	}
	wait_SPI_ltc2449();
	SPIPort.OUTSET = SPISS;  //de-select slave
	return result;
}



uint32_t Read_ltc2449(uint8_t Muxaddress) 
{ 
	uint32_t result;
	uint32_t command = 0b10100000000100000000000000000000; // Simple speed , differential input , IN+ to IN- , 000 as admux
	result = SPI_bit_bang(32, command);
	
	/*
	if(Tst_bits(result,0b01000000000000000000000000000000)) // si /EOC = 0 , la conversion est terminée 
	{
		result = 9;
	}
	else
	{
		result = 18;
	}
	*/
	
	//result = SPI_bit_bang(32, command);
	
	return result;  
	
	
 //  INT8 Index; 
  // UINT16 SDI_Data_In;  /* sdi data input word for selecting channel, speed  and resolution 
  // UINT8 temp; 
  // UINT8 End_Of_Conversion; 
   //Define a Union in order to access long words at the byte level 
  // union UChar_LongInt {UINT8 Bytes[FOUR]; INT32 SDO_Word;} Received_SPI_Data; 
  // LTC2449_CS     = LOW;          //Bring the A/D Chip Select Low 
  // PIR1bits.SSPIF = 0;            //Initialise the interrupt flag bit 
  // while (PORTCbits.RC4); //Check for end of conversion
  /* 
   for (Index = THREE; Index >= ZERO; Index--) //Get 4 bytes from the SPI 
   {                               //Note: MSB is transmitted first 
      //Intiate a bus cycle by writing to the xmit register 
      SSPBUF = 0x00; 
      while(!PIR1bits.SSPIF);        //Wait until the cycle is complete 
      PIR1bits.SSPIF = 0;            //Clear the interrupt flag bit 
      temp = SSPBUF; 
      while ((temp & 0x80) && (Index == 3)) // Check for EOC 
      { 
         temp = SSPBUF; 
      } 
      //Read the value from the receive register 
      Received_SPI_Data.Bytes[Index] = SSPBUF; 
   } // End for loop 
   
   LTC2449_CS = HIGH;                    //Bring the A/D Chip Select High 
                                     //and Start the next Conversion 
   /* Frame the SDI word to select channel,speed and resolution for 
      next conversion  
   
   SDI | 1 | 0 | EN | SGL | ODD/SIGN  A2  A1  A0 | OSR3 OSR2 OSR1 OSR0 |TWOX| 
       | 1 | 0 | 1  |  1  | <---Muxaddress------>|  0    1     1   0   | 0  | 
   SGL ---> 0 for double ended input and 1 for single ended input 
   OSR ---> 2048, Conversion rate = 110Hz, Internal clock 9Mhz 
   TWOX ---> 0, no latency 
   SDI_Data_In = ((Muxaddress << FIVE) | 0x160C) & 0x01FFF; 
   SSPBUF = SDI_Data_In & 0xFF; 
   temp = SSPBUF; // dummy read 
   SSPBUF = (SDI_Data_In >> 8) & 0xFF; 
   temp = SSPBUF; //dummy read 
   return Received_SPI_Data.SDO_Word; 
   
   */
   
   
   
}  //End of Read_ltc2449 