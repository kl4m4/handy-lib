/*
 * ssd1306.h
 *
 *  Created on: 17.03.2018
 *      Author: E6430
 */

#ifndef SSD1306_H_
#define SSD1306_H_
#include <string.h>
#include <inttypes.h>

// Pixel location in [in_buffer_index, in_page_index]
typedef struct{
	uint32_t buffIndex;
	uint8_t pageIndex;
} sPixLoc;

// Interface to hardware layer
extern void SSD1306InitHardware();
extern void SSD1306SendCommand(uint8_t);
extern void SSD1306SendData(uint8_t);


// Display config
#define SSD1306_PIXELS_X 		128	// horizontal resolution = # columns
#define SSD1306_PAGES 			8
#define SSD1306_PIX_PER_PAGE	8
#define SSD1306_PIXELS_Y 		64		// vertical resolution = PAGES x PIXELSperPAGE


// Interface
void SSD1306Init();
void SSD1306ClearBuffer();
void SSD1306SendBuffer();
void SSD1306LoadBuffer(uint8_t *buff);
uint8_t SSD1306SetPixelHorizAdr(uint8_t, uint8_t);
sPixLoc SSD1306GetPixLocHorizAdr(uint8_t, uint8_t);

uint8_t SSD1306Buffer[SSD1306_PIXELS_X * SSD1306_PAGES];





#endif /* SSD1306_H_ */
