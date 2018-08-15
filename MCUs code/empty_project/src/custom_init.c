/*
 * custom_init.c
 *
 *  Created on: 14.08.2018
 *      Author: E6430
 */
#include "custom_init.h"
#include "stm32f10x.h"


/**
  * @brief  Configures all the hardware for custom needs
  * @param  None
  * @retval None
  */
void CustomInit(void){
	CustomClockConfigHSI64();
	CustomGPIOConfig();
}

/**
  * @brief  Configures the System clock frequency to use HSI with PLL*16
  * so that SYSCLK = HCLK = PCLK2 = APB2CLK = 64MHz
  * and PCLK1 = APB1CLK = 32MHz
  * @param  None
  * @retval None
  */
void CustomClockConfigHSI64(void){
	//  -------------------------------------------------------------
	//  ---------------- System clock configuration -----------------
	//  -------------------------------------------------------------
	// HSI - already on by default

	//  --------- PLL configuration -----------
	// PLL - select PLL multiplier
	RCC->CFGR &= ~RCC_CFGR_PLLMULL;
	// all bits set = multiplier x16
	RCC->CFGR |= (RCC_CFGR_PLLMULL_0 | RCC_CFGR_PLLMULL_1 | RCC_CFGR_PLLMULL_2 | RCC_CFGR_PLLMULL_3);
	// PLL - select PLL source
	// bit cleared = HSI/2 set as input
	RCC->CFGR &= ~RCC_CFGR_PLLSRC;
	// PLL - switch on
	RCC->CR |= RCC_CR_PLLON;
	// Wait till PLL is locked
	while(!(RCC->CR & RCC_CR_PLLRDY)){}

	//  --------- FLASH latency adjustment -----------
	// apparently required
	FLASH->ACR &= ~FLASH_ACR_LATENCY;
	// according to doc.: 010 Two wait states, if 48 MHz < SYSCLK <= 72 MHz
	FLASH->ACR |= FLASH_ACR_LATENCY_2;

	//  --------- prescalers configuration -----------
	// AHB prescaler - all bits cleared - no prescaling
	RCC->CFGR &= ~RCC_CFGR_HPRE;
	// APB1 prescaler - /2 division
	RCC->CFGR &= ~RCC_CFGR_PPRE1;
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;
	// APB2 prescaler - bits cleared - no division
	RCC->CFGR &= ~RCC_CFGR_PPRE2;

	//  --------- Set PLL as clock source -----------
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_PLL;
    //RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    //RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;
	// Wait till PLL is really used as closk source
	while((RCC->CFGR & RCC_CFGR_SWS  )!= RCC_CFGR_SWS_PLL){}

	// update SystemCoreClock variable
	SystemCoreClockUpdate();
}

/**
  * @brief  Configures the GPIOs, its APB clocks, and interrupts
  * @param  None
  * @retval None
  */
void CustomGPIOConfig(void){
	//  -------------------------------------------------------------
	//  ------------- GPIO port config (port C @APB2) ---------------
	//  -------------------------------------------------------------

	//  --------- Clock for GPIO port C (APB2) -----------
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

	//  --------- Pin mode - both bits cleared - input mode -----------
	GPIOC->CRH &= ~GPIO_CRH_MODE13;

	//  -------------- Pin config - 01 - floating input ---------------
	GPIOC->CRH &= ~GPIO_CRH_CNF13;
	GPIOC->CRH |= GPIO_CRH_CNF13_0;

	//  -------------------------------------------------------------
	//  ---------------- AFIO for interrupt config ------------------
	//  -------------------------------------------------------------

	//  -------- Clock for AFIO interface (APB2) ---------
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;

	// -------- Select and configure EXTI13 line ---------
	// -------- in EXTICR4 (array index 3!)
	// -------- 0010: PC[13] pin
	AFIO->EXTICR[3] &= ~AFIO_EXTICR4_EXTI13;
	AFIO->EXTICR[3] |= AFIO_EXTICR4_EXTI13_PC;

	//  -------------------------------------------------------------
	//  -------------------- Interrupt config -----------------------
	//  -------------------------------------------------------------

	// -------- Interrupt masks ---------
	EXTI->IMR |= EXTI_IMR_MR13;

	// -------- Edge detection selection - falling ---------
	EXTI->FTSR |= EXTI_FTSR_TR13;

	// -------- Activate EXTI15_10_IRQn interrupt ---------
	// -------- EXTI15_10_IRQn falls into ISER[1], and it's
	// -------- correspondong bit is bit[8]
	// -------- In general, as in NVIC_EnableIRQ(IRQn_Type IRQn):
	// -------- NVIC->ISER[((uint32_t)(IRQn) >> 5)] = (1 << ((uint32_t)(IRQn) & 0x1F));
	NVIC->ISER[1] |= 0x0100;

}


