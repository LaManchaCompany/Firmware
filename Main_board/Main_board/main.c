/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */
#define USB_DEVICE_LOW_SPEED
/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */


#define BLINK_DELAY_MS 100
# define F_CPU 16000000UL

#include <asf.h>
#include "util/delay.h"

//mouse
//#include "udi_hid_mouse.h"
//CDC



#include "src/Soft_I2C.h"
#include "src/Soft_SPI.h"
#include "src/RC522.h"
#include "ADC_Servo.h"
#include "LT2449.h"
#include "neo_pixel.h"
#include "gobelet.h"
#include "serial_communication.h"

#include "interrupt.h"

void setUp32MhzInternalOsc(void);



//CDC /////////////////////////////////////////////
static bool my_flag_autorize_cdc_transfert = false;
bool my_callback_cdc_enable(void)
{
	my_flag_autorize_cdc_transfert = true;
	return true;
}
void my_callback_cdc_disable(void)
{
	my_flag_autorize_cdc_transfert = false;
}
/////////////////////////////////////////////////////

///////////////////////////////////////////
// command received from raspebrypis
struct Command comm; 
///////////////////////////////////////////


int main (void)
{
	
	PORTF.DIRSET = 0b00000111;
	
	/* Insert system clock initialization code here (sysclk_init()). */
	
	//setUp32MhzInternalOsc();
	
	
	//board_init();
   
    sysclk_init();
   
    irq_initialize_vectors();
   
    cpu_irq_enable();
   
   
    board_init();
    //sleepmgr_init(); // Optional

	
		/////////////////////////////////////////////////////////////////////////////
		//USB ------------- CDC ------------		
		
		udc_start();
		_delay_ms( 1000 ) ; // wait.
		udc_attach();
			
	    /////////////////////////////////////////////////////////////////////////////
		
		/////////////////////////////////////////////////////////////////////////////
		//PROTOCOL ------------- Serial communication through cdc ------------
		uint8_t code_commande[4];
		
		/////////////////////////////////////////////////////////////////////////////
	    
		/////////////////////////////////////////////////////////////////////////////
		//SPI ------------- RFID ------------
		//uint8_t RevBuffer[2];
		//uint8_t serial[4];
		//uint8_t status;
		//hard_spi_init();
		//RFID_Init();
		/////////////////////////////////////////////////////////////////////////////
		
		/////////////////////////////////////////////////////////////////////////////

		
		/////////////////////////////////////////////////////////////////////////////
		//ADC ------------- SERVO ------------
		//Servo_init();
		//	uint16_t adc_value;
		//	char* s;
		/////////////////////////////////////////////////////////////////////////////
		
		/////////////////////////////////////////////////////////////////////////////
		//I2C ------------- PUMP OUTPUT ------------
		//Soft_I2C_init()
		/////////////////////////////////////////////////////////////////////////////
		
		
		/////////////////////////////////////////////////////////////////////////////
		//SPI ------------- PESE BOUTEILLE ------------
		//LTC2449_init_port();
		//uint32_t temp = 65536;
		//char* char_temp;
		/////////////////////////////////////////////////////////////////////////////
		
		
		/////////////////////////////////////////////////////////////////////////////
		//BITBANG ------------- NEOPIXEL ------------
		init_neo_pixel();
				uint8_t r=0;
				uint8_t g=0;
				uint8_t b=0;
		uint8_t buf_r;
				uint8_t buf_g;
						uint8_t buf_b;
		/*
		init_neo_pixel();

		uint8_t led_number =0;
		*/
		/////////////////////////////////////////////////////////////////////////////
		
		/////////////////////////////////////////////////////////////////////////////
		//I/O ------------- Gobelet board ------------
		
		init_gobelet();
		uint8_t error_gob =0;
		//  0: success
		//  1: max try count error
		//  2: error rising edge time out
		char * error_gob_s;
		
		
		uint8_t time_out_gob = 50;
		uint8_t max_try_gob_count = 2;
		


		/////////////////////////////////////////////////////////////////////////////
		
		
			 while(1)
			 {
	
				
/////////////////////////////////////////////////////////////////////////////	
//SPI ------------- RFID ------------

 //pTagType - return card type
 //*                                           0x4400 = Mifare_UltraLight
 //*                                           0x0400 = Mifare_One(S50)
 //*                                           0x0200 = Mifare_One(S70)
 //*                                           0x0800 = Mifare_Pro(X)
 //*                                           0x4403 = Mifare_DESFire


//#define 	MI_OK                 0
//#define 	MI_NOTAGERR           1
//#define 	MI_ERR                2
								
				//uint8_t RevBuffer[3];
			
				//status = MFRC522_Request(PICC_REQIDL,&RevBuffer[0]);  //Search card, return card types
				//status =  MFRC522_Anticoll(&serial);
			
				
				//if(status == 0)
				//{
					//
					//udi_cdc_write_buf(serial,4);
					//udi_cdc_putc('\r');
					//PORTF.OUTSET = 0b00000111 ; // set the output high.)
				//}
				//if(status == 1)
				//{
					//PORTF.OUTSET = 0b00000110 ; // set the output high.)
				//}
				//if(status == 2)
				//{
					//PORTF.OUTSET = 0b00000101 ; // set the output high.)
				//}
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//USB ------------- CDC ------------
		

			//udi_cdc_putc();
			//udi_cdc_write_buf();
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
//I/O ------------- Gobelet board ------------
			//	trig_gobelet();
				

			
			comm = get_next_command(code_commande,NULL,10);
			
			if(compare_string("COCK", code_commande,4, 0) == 1 )
			{
				udi_cdc_write_buf ("CACK\r", 5);
			}
			
			//error_gob = give_me_a_gob(max_try_gob_count,time_out_gob);
		
/////////////////////////////////////////////////////////////////////////////
				


/////////////////////////////////////////////////////////////////////////////	
//BLINK
				for(char i=0; i<4;i++)
				{
					PORTF.OUTSET = 0b00000111 ;
					_delay_ms( BLINK_DELAY_MS ) ; // wait.
					PORTF.OUTCLR = 0b00000111 ;
					_delay_ms( BLINK_DELAY_MS ) ; // wait.
				}
				
/////////////////////////////////////////////////////////////////////////////
	
				
/////////////////////////////////////////////////////////////////////////////
//ADC ------------- SERVO ------------
						
				//	adc_value = align_servo(800);
/////////////////////////////////////////////////////////////////////////////
		 
/////////////////////////////////////////////////////////////////////////////
//I2C ------------- PUMP OUTPUT ------------		 
		 				//Soft_I2C_write_port(q);
/////////////////////////////////////////////////////////////////////////////						 
						 
						

/////////////////////////////////////////////////////////////////////////////
//SPI ------------- PESE BOUTEILLE ------------		 
			 //udi_cdc_write_buf(Read_ltc2449(0), 32); // PAS FINI
			 // temp = Read_ltc2449(0);
			 
			 
			 //if(temp != 0) udi_cdc_putc('B');
			
			 //udi_cdc_write_buf(temp,32);
			 
			//	udi_cdc_putc('L');
			//	udi_cdc_putc('\n');

			 
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
//BITBANG ------------- NEO_PIXEL------------

		//	udi_cdc_read_buf(&buf_r, 1);
			
		//	udi_cdc_read_buf(&buf_g, 1);
		//	udi_cdc_read_buf(&buf_b, 1);
		//	udi_cdc_putc(buf_r);
		//	udi_cdc_putc('\n');
				

						
//cli();
//test_neo_pixel(buf_r,buf_g,buf_b);

		
//sei();		
/*
r =  r + 4;
g =  g + 1;
b =  b + 2;
if(r>255) r= 0;
if(b>255) b=0;
if(g>255) g=0;

cli();
for(uint8_t j=0;j<led_number;j++)
{
	test_neo_pixel(r,g,b);
}

test_neo_pixel(r,g,b);
test_neo_pixel(2,r,25);
reset_neo_pixel();
sei();
led_number ++;
if(led_number ==30) led_number = 0;
*/
//temp_RBG = temp_RBG +10;
//if(temp_RBG > 0xFFFFFF)
//temp_RBG=0;
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////		 
		 



			
		 
		 
		 
			 }
	 
	 
	
}





void setUp32MhzInternalOsc()
{
	OSC_CTRL |= OSC_RC32MEN_bm; //Setup 32Mhz crystal
	
	while(!(OSC_STATUS & OSC_RC32MRDY_bm));
	
	CCP = CCP_IOREG_gc; //Trigger protection mechanism
	CLK_CTRL = CLK_SCLKSEL_RC32M_gc; //Enable internal  32Mhz crystal
	
	
}

