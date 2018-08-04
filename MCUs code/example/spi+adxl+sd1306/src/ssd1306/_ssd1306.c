/*
 * ssd1306.c
 *
 *  Created on: 17.03.2018
 *      Author: E6430
 */

#include <ssd1306/_ssd1306.h>


void SSD1306Init(){


	  SSD1306SendCommand(0xAE);//wy��cz panel OLED
	  SSD1306SendCommand(0x00);//adres kolumny LOW
	  SSD1306SendCommand(0x10);//adres kolumny HIGH
	  SSD1306SendCommand(0x40);//adres startu linii
	  SSD1306SendCommand(0x20);//tryb adresowania strony
	  //SSD1306SendCommand(0x01);//vertical mode !!!
	  SSD1306SendCommand(0x00);//horizontal mode !!!
	  SSD1306SendCommand(0x81);//ustaw kontrast
	  SSD1306SendCommand(0xCF);
	  SSD1306SendCommand(0xA1);//ustaw remapowanie
	  SSD1306SendCommand(0xC8);//kierunek skanowania
	  SSD1306SendCommand(0xA8);//ustaw multiplex ratio
	  SSD1306SendCommand(0x3F);//1/64
	  SSD1306SendCommand(0xD3);//ustaw display offset
	  SSD1306SendCommand(0x00);//bez offsetu
	  SSD1306SendCommand(0xD5);//ustaw divide ratio/cz�stotliwo�� oscylatora
	  SSD1306SendCommand(0x80);//100ramek/sec
	  SSD1306SendCommand(0xD9);//ustaw okres pre charge
	  SSD1306SendCommand(0xF1);//pre charge 15 cykli, discharge 1 cykl
	  SSD1306SendCommand(0xDA);//konfiguracja wyprowadze� sterownika
	  SSD1306SendCommand(0x12);
	  SSD1306SendCommand(0xDB);//ustawienie vcomh
	  SSD1306SendCommand(0x40);
	  SSD1306SendCommand(0x8D);//ustawienie Charge Pump
	  SSD1306SendCommand(0x14);
	  SSD1306SendCommand(0xA4);//"pod��czenie" zawarto�ci RAM do panelu OLED
	  SSD1306SendCommand(0xA6);//wy��czenie inwersji wy�wietlania
	  SSD1306SendCommand(0xAF);//w��cza wy�wietlacz

}

void SSD1306ClearBuffer(){
	memset(SSD1306Buffer, 0x00, SSD1306_PIXELS_X * SSD1306_PAGES);
}

void SSD1306SendBuffer(){
	// Select chip=0
	//GPIO_ResetBits(SSD1306_CONTROL_PORT, SSD1306_CS_GPIOPIN);
	// Set data mode - DC=high
	//GPIO_SetBits(SSD1306_CONTROL_PORT, SSD1306_DC_GPIOPIN);

	for(unsigned short byte = 0; byte < SSD1306_PIXELS_X * SSD1306_PAGES; byte++){
		SSD1306SendData(SSD1306Buffer[byte]);
	}
}

void SSD1306LoadBuffer(uint8_t *buff){
	memcpy(SSD1306Buffer,buff, SSD1306_PIXELS_X * SSD1306_PAGES);
}

sPixLoc SSD1306GetPixLocHorizAdr(uint8_t x, uint8_t y){
	uint8_t page = y/SSD1306_PIX_PER_PAGE;
	sPixLoc ret_val;
	ret_val.buffIndex = page * SSD1306_PIXELS_X + x;
	ret_val.pageIndex = y - page*SSD1306_PIX_PER_PAGE;
	return ret_val;
}

uint8_t SSD1306SetPixelHorizAdr(uint8_t x, uint8_t y){
	if(x >= SSD1306_PIXELS_X){
		return 0;
	}
	if(y >= SSD1306_PIXELS_Y){
		return 0;
	}
	sPixLoc pix_loc = SSD1306GetPixLocHorizAdr(x, y);
	SSD1306Buffer[pix_loc.buffIndex] |= 1 << pix_loc.pageIndex;
	return 1;
}
