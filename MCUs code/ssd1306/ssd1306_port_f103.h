/*
 * ssd1306_port_f103.h
 *
 *  Created on: 11.06.2018
 *      Author: E6430
 */

#ifndef SSD1306_SSD1306_PORT_F103_H_
#define SSD1306_SSD1306_PORT_F103_H_

#include "stm32f10x.h"
#include <inttypes.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_spi.h>

// Functions that will be given to SSD1306Init
void SSDSelectChip();
void SSDDeSelectChip();
void SSDSetDataMode();
void SSDSetCommandMode();
void SSDSendByte(uint8_t);
void SSDResetActive();
void SSDResetInactive();
void SSDDelay(uint8_t);


#endif /* SSD1306_SSD1306_PORT_F103_H_ */
