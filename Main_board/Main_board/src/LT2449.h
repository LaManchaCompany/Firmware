/*
 * LT2449.h
 *
 * Created: 12-11-16 10:21:22
 *  Author: Jérémie
 */ 


#ifndef LT2449_H_
#define LT2449_H_



void LTC2449_init_port();
void wait_SPI_ltc2449();
void SPI_togle_sck();
uint32_t SPI_bit_bang(uint8_t lenght, uint32_t data_out);
uint32_t Read_ltc2449(uint8_t Muxaddress);

#endif /* LT2449_H_ */