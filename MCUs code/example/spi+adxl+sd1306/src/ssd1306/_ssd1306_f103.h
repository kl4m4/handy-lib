/*
 * ssd1306_f103.h
 *
 *  Created on: 17.03.2018
 *      Author: E6430
 */

#include "stm32f10x.h"

#ifndef SSD1306_F103_H_
#define SSD1306_F103_H_

#define SSD1306_CONTROL_PORT GPIOC
#define _RCC_APB2Periph_GPIO RCC_APB2Periph_GPIOC
#define SSD1306_RESET_GPIOPIN GPIO_Pin_2
#define SSD1306_RESET_GPIO GPIOC
#define SSD1306_DC_GPIOPIN GPIO_Pin_1
#define SSD1306_DC_GPIO GPIOC
#define SSD1306_CS_GPIOPIN GPIO_Pin_0
#define SSD1306_CS_GPIO GPIOC

void SSD1306InitHardware();
void SSD1306SendCommand(uint8_t);
void SSD1306SendData(uint8_t);


extern void Delay(__IO uint32_t);

#endif /* SSD1306_F103_H_ */
