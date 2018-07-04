/*
 * ssd1306_port_f103.c
 *
 *  Created on: 14.06.2018
 *      Author: E6430
 */

#include "ssd1306_port_f103.h"
#include "spi\spi_hw.h"

void SSDSelectChip(){
	GPIO_ResetBits(CONTROL_PORT, CS_SSD_pin);
	return;
}

void SSDDeSelectChip(){
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
	GPIO_SetBits(CONTROL_PORT, CS_SSD_pin);
	return;
}

void SSDSetDataMode(){
	GPIO_SetBits(CONTROL_PORT, DC_pin);
	return;
}

void SSDSetCommandMode(){
	GPIO_ResetBits(CONTROL_PORT, DC_pin);
	return;
}

void SSDSendByte(uint8_t byte){
	while(!SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE)){}
	SPI_I2S_SendData(SPI1, byte);
	return;
}

void SSDResetActive(){
	GPIO_ResetBits(CONTROL_PORT, RST_pin);
	return;
}

void SSDResetInactive(){
	GPIO_SetBits(CONTROL_PORT, RST_pin);
	return;
}
void SSDDelay(uint8_t milis){
	Delay(milis);
	return;
}
