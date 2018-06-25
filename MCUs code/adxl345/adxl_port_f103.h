/*
 * adxl_port_f103.h
 *
 *  Created on: 03.06.2018
 *      Author: E6430
 */

#ifndef ADXL345_ADXL_PORT_F103_H_
#define ADXL345_ADXL_PORT_F103_H_

#include <inttypes.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_spi.h>

#define CS_CHANNEL 	2

// Interface to SPI hardware - these are implemented in part of code responsible for SPI communication

// Interface to ADXL logic
//void ADXL_HW_SelectChip();
//void ADXL_HW_DeSelectChip();
//void ADXL_HW_Send(uint8_t);
//uint8_t ADXL_HW_Read();

// Higher level interface
uint8_t ADXL_ReadNRegs(uint8_t first, uint8_t number, uint8_t *buff);
void ADXL_WriteReg(uint8_t reg, uint8_t value);

#endif /* ADXL345_ADXL_PORT_F103_H_ */
