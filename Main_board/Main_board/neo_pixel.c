//NEO PIXEL DATA PIN: port E 6 

//Symbol	Parameter				Min		Typical		Max		Units
//T0H	0 code ,high voltage time	200		350			500		ns
//T1H	1 code ,high voltage time	550		700			850		ns
//T0L	0 code , low voltage time	650		800			950		ns
//T1L	1 code ,low voltage time	450		600			750		ns
//RES	low voltage time			50,000						ns





#include "neo_pixel.h"
#include <avr/io.h>
#include "util/delay.h"

init_neo_pixel()
{
		// data pin as output
		//	OUT/IN		 76543210
		PORTE.DIRSET = 0b11100000;
		// set data pin to 0;
		//	OUT/IN		 76543210
		PORTE.OUTCLR = 0b11100000;
}

test_neo_pixel(uint8_t r, uint8_t g, uint8_t b)
{
	//	OUT/IN		 76543210
	PORTE.OUTCLR = 0b01000000;
	
	send_byte(g);
	send_byte(r);
	send_byte(b);
	
	
}

send_byte(uint8_t b)
{
	
	for(uint8_t i=8; i>0; i--)
	{
		if(b & 0x01 <<i )
		{
			one();
		}
		else
		{
			zero();
		}
		
	}
}


zero()
{
	//T0H 5* period of 62.5ns = 312,5ns
	//	OUT/IN		 76543210
	PORTE.OUTSET = 0b01000000;
	asm ("nop");
	asm ("nop");
	asm ("nop");
	asm ("nop");

	
	//T0L 12* period of 62.5ns = 750ns
	//	OUT/IN		 76543210
	PORTE.OUTCLR = 0b01000000;

}

one()
{
	//T1H 11* period of 62.5ns = 687,5ns
	//	OUT/IN		 76543210
	PORTE.OUTSET = 0b01000000;
	asm ("nop");
	asm ("nop");
	asm ("nop");
	asm ("nop");
	asm ("nop");
	asm ("nop");
	asm ("nop");
	asm ("nop");
	asm ("nop");
	asm ("nop");
	asm ("nop");
		
	//T1L 10* period of 62.5ns = 625ns
	//	OUT/IN		 76543210
	PORTE.OUTCLR = 0b01000000;
	
	
}

end()
{
	//	OUT/IN		 76543210
	PORTE.OUTCLR = 0b01000000;	
}

reset_neo_pixel()
{
	//	OUT/IN		 76543210
	PORTE.OUTCLR = 0b01000000;
	_delay_ms(54);
	end();	
}