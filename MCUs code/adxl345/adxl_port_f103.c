/*
 * adxl_port_f103.c
 *
 *  Created on: 03.06.2018
 *      Author: E6430
 */

#include "adxl_port_f103.h"
#include "spi\spi_hw.h"

void ADXL_HW_SelectChip(){
	GPIO_ResetBits(CONTROL_PORT, CS_ADXL_pin);
}

void ADXL_HW_DeSelectChip(){
	GPIO_SetBits(CONTROL_PORT, CS_ADXL_pin);
}

void ADXL_HW_Send(uint8_t byte){
	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)){}
	SPI_I2S_SendData(SPI1, byte);
}

uint8_t ADXL_HW_Read(){
	//while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)){}
	return SPI_I2S_ReceiveData(SPI1);
}

uint8_t ADXL_ReadNRegs(uint8_t first, uint8_t number, uint8_t *buff){
	if(number <1){return 0;}
	if(buff == 0){return 0;}
	uint8_t regs_read = 0;
	// set READ bit
	first |= 0b10000000;
	// set MULTIBYTE bit if needed
	if(number > 1){
		first |= 0b01000000;
	}
	// select chip
	GPIO_ResetBits(CONTROL_PORT, CS_ADXL_pin);
	// transmit register adress
	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)){}
	SPI_I2S_SendData(SPI1, first);
	// read whatever, just to clear RXNE flag
	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)){}
	SPI_I2S_ReceiveData(SPI1);

	// dirty hack - insert one extra tx-rx cycle, otherwise all the bytes are delayed by 1
	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)){}
	SPI_I2S_SendData(SPI1, 0);
	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)){}
	SPI_I2S_ReceiveData(SPI1);

	// read as many regs as requested
	for(regs_read = 0; regs_read < number; regs_read++){
		// transmit 0's
		while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)){}
		SPI_I2S_SendData(SPI1, 0);
		// read back
		while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)){}
		buff[regs_read] = SPI_I2S_ReceiveData(SPI1);
	}

	//deselect chip
	GPIO_SetBits(CONTROL_PORT, CS_ADXL_pin);
	return regs_read+1;		// no. of registers read
}

void ADXL_WriteReg(uint8_t reg, uint8_t value){
	// for write transmission, no multibyte, clear 2 first bits
	reg &= 0b00111111;
	// select chip
	GPIO_ResetBits(CONTROL_PORT, CS_ADXL_pin);
	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)){}
	SPI_I2S_SendData(SPI1, reg);
	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)){}
	SPI_I2S_SendData(SPI1, value);
	// wait some time until transmission ends
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY)){}
	// deselect chip
	GPIO_SetBits(CONTROL_PORT, CS_ADXL_pin);
}
