//#ifndef __MOJE_PODRECZNE_H
//#define __MOJE_PODRECZNE_H

#include "stm32f10x.h"

// Podrêczne kawa³ki kodu
//Inicjalizacja pinu jako wyjœcia
// Przyk³ad u¿ycia:
// CONFIG_OUTPUT_PIN(RCC_APB2Periph_GPIOA,GPIOA,GPIO_Pin_5);
#define CONFIG_OUTPUT_PIN(_RCC_APB2Periph_GPIO,_GPIO,_GPIO_Pin) 	\
	RCC_APB2PeriphClockCmd(_RCC_APB2Periph_GPIO, ENABLE); 			\
	GPIO_InitTypeDef _gpio;GPIO_StructInit(&_gpio); 				\
	_gpio.GPIO_Pin = _GPIO_Pin; 									\
	_gpio.GPIO_Mode = GPIO_Mode_Out_PP;								\
	GPIO_Init(_GPIO, &_gpio)
