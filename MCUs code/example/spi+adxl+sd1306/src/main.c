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
#include <inttypes.h>
#include "main.h"
#include "spi\spi_hw.h"
#include <stm32f10x_gpio.h>
#include "adxl345\adxl_port_f103.h"
#include "adxl345\adxl345.h"
#include "adxl345\adxl345_regs.h"
#include "ssd1306\ssd1306.h"
#include "ssd1306\ssd1306_port_f103.h"
#include "typewriter\typewriter.h"
#include "typewriter\TomThumb.h"
#include "typewriter\FreeSerifBold12pt7b.h"

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

sSSDdisplay disp;

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
  
  //spi config/init
  SPI_HW_Init();

  sADXLSPI adxl_link;
  ADXL_Init(&adxl_link, &ADXL_ReadNRegs, &ADXL_WriteReg);
  ADXL_StartMeasurement(&adxl_link);




  SSD1306Init(&disp, &SSDSelectChip, &SSDDeSelectChip, &SSDSetDataMode, &SSDSetCommandMode, &SSDSendByte, &SSDResetActive, &SSDResetInactive, &SSDDelay);
  SSD1306SendBuffer(&disp);



  // linijki tekstu
  char linebuff1[16];
  //char linebuff2[16];
  //char linebuff3[16];
  sTextLine line1;
  //sTextLine line2;
  //sTextLine line3;
  sTextLineInit(&line1, linebuff1, 30, 40, 10, &FreeSerifBold12pt7b);
  //sTextLineInit(&line2, linebuff2, 20, 35, 10, &TomThumb);
  //sTextLineInit(&line3, linebuff3, 20, 50, 10, &TomThumb);

  uint8_t tmpBuff[] = {0, 0, 0, 0, 0, 0};

  /* Infinite loop */
  while (1)
  {

	  ADXL_GetDataRegs(&adxl_link);
	  char tmp[10];
	  TypeWriterFloat2Ascii(&tmp, ADXL_GetAccX(&adxl_link), 2);
	  sTextLineSetText(&line1, tmp);
	  //TypeWriterFloat2Ascii(&tmp, ADXL_GetAccY(&adxl_link), 2);
	  //sTextLineSetText(&line2, tmp);
	  //TypeWriterFloat2Ascii(&tmp, ADXL_GetAccZ(&adxl_link), 2);
	  //sTextLineSetText(&line3, tmp);
	  SSD1306ClearBuffer(&disp);
	  sTextLineDraw(&line1);
	  //sTextLineDraw(&line2);
	  //sTextLineDraw(&line3);
	  SSD1306SendBuffer(&disp);

	  Delay(50);
  }
}

// zmapowana funkcja ustawiaj¹ca pixel, na potrzeby typerwiter.h
void DisplaySetPixelHorizAdr(uint8_t x,uint8_t y){
	SSD1306SetPixelHorizAdr(&disp, x, y);
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
