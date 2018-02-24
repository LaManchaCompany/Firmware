/*
 * Soft_SPI.h
 *
 * Created: 12-09-16 19:46:46
 *  Author: Jérémie
 */ 
#include <avr/io.h>
#include <sysclk.h>


#ifndef SOFT_SPI_H_
#define SOFT_SPI_H_

void wait_SPI();
void hard_spi_init();
void spiclose(void);
uint8_t spi(uint8_t val);


#endif  SOFT_SPI_H_ 