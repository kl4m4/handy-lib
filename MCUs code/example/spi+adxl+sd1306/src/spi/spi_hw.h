/*
 * spi_hw.h
 *
 *  Created on: 03.06.2018
 *      Author: E6430
 */

#ifndef SPI_SPI_HW_H_
#define SPI_SPI_HW_H_

#include <stm32f10x.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_spi.h>
#include <inttypes.h>

// za³o¿enia:
// interface SPI1
// 4 piny CS
// 1 pin data/command
// 1 pin reset

// PA0 - CS_SSD
// PA1 - CS_ADXL
// PA2 - DATA/nCOMMAND
// PA3 - nRST

// wszystko w obrêbie APB2!

#define USE_CONTROL_PORT_C

#define CS_SSD_pin 		GPIO_Pin_0
#define CS_ADXL_pin		GPIO_Pin_1
#define DC_pin 			GPIO_Pin_2
#define RST_pin 		GPIO_Pin_3

#ifdef USE_CONTROL_PORT_C
	#define CONTROL_PORT 		GPIOC
	#define _APB2Periph_GPIO 	RCC_APB2Periph_GPIOC
#elif
	while(1){}
#endif

void SPI_HW_Init();

void SPI_HW_Send(uint8_t);
uint8_t SPI_HW_GetData();
uint8_t SPI_HW_ReadyToTx();
uint8_t SPI_HW_RxDataReady();

#endif /* SPI_SPI_HW_H_ */
