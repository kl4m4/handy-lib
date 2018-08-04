/**
  ******************************************************************************
  * @file    main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    11-February-2014
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup IO_Toggle
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static __IO uint32_t TimingDelay;
uint8_t BlinkSpeed = 0;
/* Private function prototypes -----------------------------------------------*/
RCC_ClocksTypeDef RCC_Clocks;
/* Private functions ---------------------------------------------------------*/

/**
  * @brief   Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f10x_md.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f10x.c file
     */  
  
  /* SysTick end of count event each 1ms */
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);
  
  /* Initialize LED2 */
  STM_EVAL_LEDInit(LED2);
  
  /* Initialize User_Button on STM32NUCLEO */
  STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_EXTI);   
  
  /* Initiate Blink Speed variable */ 
  BlinkSpeed = 0;
  
  // apb1 clock
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  TIM_TimeBaseInitTypeDef timebasestruct;
  TIM_TimeBaseStructInit(&timebasestruct);
  timebasestruct.TIM_ClockDivision = 0;
  timebasestruct.TIM_CounterMode = TIM_CounterMode_Up;
  timebasestruct.TIM_Period = 32786;
  timebasestruct.TIM_Prescaler = 487;
  timebasestruct.TIM_RepetitionCounter = 0;

  TIM_TimeBaseInit(TIM2, &timebasestruct);


  //TIM_CounterModeConfig
  //TIM_InternalClockConfig
  //TIM_PrescalerConfig
  //TIM_ARRPreloadConfig
  //TIM_SetAutoreload

  //TIM_OCInitTypeDef oc1;
  //TIM_OCStructInit(oc1);

  //TIM_OC1Init(TIM2, oc1);

  TIM_SetCompare1(TIM2, 5000);
  TIM_SetCompare2(TIM2, 10000);



  TIM_ClearFlag(TIM2, TIM_FLAG_Update);
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
  TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);


  TIM_Cmd(TIM2, ENABLE);

  // tim2 interrupt enable
  NVIC_InitTypeDef nvicStructure;
  nvicStructure.NVIC_IRQChannel = TIM2_IRQn;
  nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
  nvicStructure.NVIC_IRQChannelSubPriority = 1;
  nvicStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&nvicStructure);

  /* Infinite loop */
  while (1)
  {
//    /* Test on blink speed */
//    if(BlinkSpeed == 0)
//    {
//      /*LED2 Toggle each 50ms*/
//      STM_EVAL_LEDToggle(LED2);
//      Delay(50);
//    }
//    else if(BlinkSpeed == 1)
//    {
//      STM_EVAL_LEDToggle(LED2);
//      /*LED2 Toggle each 200ms */
//      Delay(200);
//    }
  }
}

/**
* @brief  Inserts a delay time.
* @param  nTime: specifies the delay time length, in 1 ms.
* @retval None
*/
void Delay(__IO uint32_t nTime)
{
  TimingDelay = nTime;
  
  while(TimingDelay != 0);
}

/**
* @brief  Decrements the TimingDelay variable.
* @param  None
* @retval None
*/
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}


#ifdef  USE_FULL_ASSERT

/**
* @brief  Reports the name of the source file and the source line number
*         where the assert_param error has occurred.
* @param  file: pointer to the source file name
* @param  line: assert_param error line source number
* @retval None
*/
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
  ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  
  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
* @}
*/


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
