#include "spi_hw.h"


void SPI_HW_Init(){
	// w³¹czenie zegara dla portu na którym s¹ piny SPI i kontrolne
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	// w³¹czenie zegara dla modu³u SPI
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	// w³¹czenie zegara dla modu³u AFIO (alternate function io)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	// konfiguracja pinów kontrolnych
	GPIO_InitTypeDef gpio;
	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin = CS_SSD_pin | CS_ADXL_pin | DC_pin | RST_pin;
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &gpio);
		// konfiguracja pinów transmisji SPI
	GPIO_StructInit(&gpio);
	gpio.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7; // SCK, MISO, MOSI
	gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio);


	// konfiguracja samego SPI
	SPI_InitTypeDef spi;
	SPI_StructInit(&spi);
	spi.SPI_Mode = SPI_Mode_Master;
	spi.SPI_NSS = SPI_NSS_Soft;
	spi.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	spi.SPI_CPOL = SPI_CPOL_High; 		// gdy brak transmisji, zegar ma stan wysoki
	spi.SPI_CPHA = SPI_CPHA_2Edge;		// próbkowanie bitu na drugie (czyli narastaj¹ce) zbocze zegara, czyli w po³owie trwania bitu
	SPI_Init(SPI1, &spi);
	SPI_Cmd(SPI1, ENABLE);


}



/*
uint8_t SPI_HW_SendReceive(uint8_t byte){
	SPI_I2S_SendData(SPI1, byte);
	return SPI_I2S_ReceiveData(SPI1);
}
*/

void SPI_HW_Send(uint8_t byte){
	SPI_I2S_SendData(SPI1, byte);
}

uint8_t SPI_HW_GetData(){
	return SPI_I2S_ReceiveData(SPI1);
}

uint8_t SPI_HW_ReadyToTx(){
	return SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE);
}

uint8_t SPI_HW_RxDataReady(){
	return SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE);
}
