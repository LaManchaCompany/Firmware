/*
 * Soft_I2C.h
 *
 * Created: 28-08-16 11:42:50
 *  Author: Jérémie
 */ 


#ifndef SOFT_I2C_H_
#define SOFT_I2C_H_

//#include  "avr\iox128a3u.h"
#include <avr/io.h>


void wait();
void Soft_I2C_Writing_condition();
void Soft_I2C_Reading_condition();
void Soft_I2C_init();
void Soft_I2C_Start();
void Soft_I2C_Stop();
void Soft_I2C_scl_High();
void Soft_I2C_scl_Low();
void Soft_I2C_sda_High();
void Soft_I2C_sda_Low();
void Soft_I2C_write_bit(uint8_t x, uint8_t m);
static void Soft_I2C_write_uint8(uint8_t x);
void Soft_I2C_write_port(uint8_t x);



#endif /* SOFT_I2C_H_ */