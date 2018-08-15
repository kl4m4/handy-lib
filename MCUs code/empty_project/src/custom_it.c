/*
 * custom_it.c
 *
 *  Created on: 15.08.2018
 *      Author: E6430
 */

#include "stm32f10x.h"

//  -------------------------------------------------------------
//  ---------------- Custom interrupt routines ------------------
//  -------------------------------------------------------------


/**
  * @brief  Interrupt routine for EXTI10..15
  * @param  None
  * @retval None
  */
void EXTI15_10_IRQHandler(void){
	EXTI->PR |= EXTI_PR_PR13;
	// do something...
}
