/*
 * ssd1306_f103.c
 *
 *  Created on: 17.03.2018
 *      Author: E6430
 */

#include <ssd1306/_ssd1306_f103.h>

void SSD1306InitHardware(){
	//
	// Hardware layer initialization
	//
	// Control pins:
	// CS - Chip Select
	// DC - Data/Command selection
	// RST - chip reset


	// Init RST pin
	GPIO_InitTypeDef gpio;
	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin = SSD1306_RESET_GPIOPIN;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SSD1306_CONTROL_PORT, &gpio);
	// instant reset
	GPIO_ResetBits(SSD1306_CONTROL_PORT, SSD1306_RESET_GPIOPIN);

	// Init DC pin
	//GPIO_InitTypeDef gpio;
	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin = SSD1306_DC_GPIOPIN;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SSD1306_CONTROL_PORT, &gpio);
	// instant pull down
	GPIO_ResetBits(SSD1306_CONTROL_PORT, SSD1306_DC_GPIOPIN);




	// Reset at startup
	// CS=1
	GPIO_SetBits(SSD1306_CONTROL_PORT, SSD1306_CS_GPIOPIN);
	// DC=0
	GPIO_ResetBits(SSD1306_CONTROL_PORT, SSD1306_DC_GPIOPIN);
	// RESET=1
	GPIO_SetBits(SSD1306_CONTROL_PORT, SSD1306_RESET_GPIOPIN);
	Delay(10);
	// RESET=0
	GPIO_ResetBits(SSD1306_CONTROL_PORT, SSD1306_RESET_GPIOPIN);
	Delay(100);
	// RESET=1
	GPIO_SetBits(SSD1306_CONTROL_PORT, SSD1306_RESET_GPIOPIN);
	Delay(100);
}

void SSD1306SendCommand(uint8_t uiCommand){
	// Select chip=0
	GPIO_ResetBits(SSD1306_CONTROL_PORT, SSD1306_CS_GPIOPIN);
	// Set command mode - DC=low
	GPIO_ResetBits(SSD1306_CONTROL_PORT, SSD1306_DC_GPIOPIN);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI1, uiCommand);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
	// Select chip=1
	GPIO_SetBits(SSD1306_CONTROL_PORT, SSD1306_CS_GPIOPIN);

}

void SSD1306SendData(uint8_t uiData){
	// Select chip=0
	GPIO_ResetBits(SSD1306_CONTROL_PORT, SSD1306_CS_GPIOPIN);
	// Set data mode - DC=high
	GPIO_SetBits(SSD1306_CONTROL_PORT, SSD1306_DC_GPIOPIN);
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI1, uiData);

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BSY) == SET);
	// Select chip=1
	GPIO_SetBits(SSD1306_CONTROL_PORT, SSD1306_CS_GPIOPIN);
}


