/**
 * \file
 *
 * \brief User board initialization template
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include <asf.h>
#include <board.h>
#include <conf_board.h>

void board_init(void)
{
	/* This function is meant to contain board-specific initialization code
	 * for, e.g., the I/O pins. The initialization can rely on application-
	 * specific board configuration, found in conf_board.h.
	 */
	
	
	//16MHz external crystal
	OSC_XOSCCTRL = OSC_FRQRANGE_12TO16_gc | OSC_XOSCSEL_XTAL_16KCLK_gc;
	//Enable external oscillator
	OSC_CTRL |= OSC_XOSCEN_bm;
	//Wait for clock stabilization
	while(!(OSC_STATUS & OSC_XOSCRDY_bm));
	// Selects clock system as external clock
	// through change protection mechanism
	CCP = CCP_IOREG_gc;
	CLK_CTRL = CLK_SCLKSEL_XOSC_gc;
	
}
