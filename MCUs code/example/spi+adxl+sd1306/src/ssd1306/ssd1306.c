/*
 * ssd1306.c
 *
 *  Created on: 14.06.2018
 *      Author: E6430
 */


#include "ssd1306.h"

void SSD1306Init(sSSDdisplay *disp, void (*ptrSel)(), void (*ptrDesel)(), void (*ptrSetData)(), void (*ptrSetCommand)(), void (*ptrSendByte)(uint8_t), void (*ptrResON)(), void (*ptrResOFF)(), void (*ptrDelay)(uint8_t)){
	disp->fpSelectChip = ptrSel;
	disp->fpDeSelectChip = ptrDesel;
	disp->fpSetData = ptrSetData;
	disp->fpSetCommand = ptrSetCommand;
	disp->fpSendByte = ptrSendByte;
	disp->fpResetActive = ptrResON;
	disp->fpResetInactive = ptrResOFF;
	disp->fpDelay = ptrDelay;
	SSD1306ClearBuffer(disp);
	// initial reset
	disp->fpDeSelectChip();
	disp->fpSetCommand();
	disp->fpResetInactive();
	disp->fpDelay(10);
	disp->fpResetActive();
	disp->fpDelay(50);
	disp->fpResetInactive();

	// display configuration
	  SSD1306SendCommand(disp, 0xAE);//wy��cz panel OLED
	  SSD1306SendCommand(disp, 0x00);//adres kolumny LOW
	  SSD1306SendCommand(disp, 0x10);//adres kolumny HIGH
	  SSD1306SendCommand(disp, 0x40);//adres startu linii
	  SSD1306SendCommand(disp, 0x20);//tryb adresowania strony
	  //SSD1306SendCommand(disp, 0x01);//vertical mode !!!
	  SSD1306SendCommand(disp, 0x00);//horizontal mode !!!
	  SSD1306SendCommand(disp, 0x81);//ustaw kontrast
	  SSD1306SendCommand(disp, 0xCF);
	  SSD1306SendCommand(disp, 0xA1);//ustaw remapowanie
	  SSD1306SendCommand(disp, 0xC8);//kierunek skanowania
	  SSD1306SendCommand(disp, 0xA8);//ustaw multiplex ratio
	  SSD1306SendCommand(disp, 0x3F);//1/64
	  SSD1306SendCommand(disp, 0xD3);//ustaw display offset
	  SSD1306SendCommand(disp, 0x00);//bez offsetu
	  SSD1306SendCommand(disp, 0xD5);//ustaw divide ratio/cz�stotliwo�� oscylatora
	  SSD1306SendCommand(disp, 0x80);//100ramek/sec
	  SSD1306SendCommand(disp, 0xD9);//ustaw okres pre charge
	  SSD1306SendCommand(disp, 0xF1);//pre charge 15 cykli, discharge 1 cykl
	  SSD1306SendCommand(disp, 0xDA);//konfiguracja wyprowadze� sterownika
	  SSD1306SendCommand(disp, 0x12);
	  SSD1306SendCommand(disp, 0xDB);//ustawienie vcomh
	  SSD1306SendCommand(disp, 0x40);
	  SSD1306SendCommand(disp, 0x8D);//ustawienie Charge Pump
	  SSD1306SendCommand(disp, 0x14);
	  SSD1306SendCommand(disp, 0xA4);//"pod��czenie" zawarto�ci RAM do panelu OLED
	  SSD1306SendCommand(disp, 0xA6);//wy��czenie inwersji wy�wietlania
	  SSD1306SendCommand(disp, 0xAF);//w��cza wy�wietlacz

	return;
}

void SSD1306ClearBuffer(sSSDdisplay *disp){
	memset(disp->PixBuffer, 0x00, SSD1306_PIXELS_X * SSD1306_PAGES);
	return;
}

void SSD1306SendBuffer(sSSDdisplay *disp){
	for(unsigned short byte = 0; byte < SSD1306_PIXELS_X * SSD1306_PAGES; byte++){
		SSD1306SendData(disp, disp->PixBuffer[byte]);
	}
	return;
}

uint8_t SSD1306SetPixelHorizAdr(sSSDdisplay *disp, uint8_t x, uint8_t y){
	if(x >= SSD1306_PIXELS_X){
		return 0;
	}
	if(y >= SSD1306_PIXELS_Y){
		return 0;
	}
	sPixLoc pix_loc = SSD1306GetPixLocHorizAdr(x, y);
	disp->PixBuffer[pix_loc.buffIndex] |= 1 << pix_loc.pageIndex;
	return 1;
}

sPixLoc SSD1306GetPixLocHorizAdr(uint8_t x, uint8_t y){
	uint8_t page = y/SSD1306_PIX_PER_PAGE;
	sPixLoc ret_val;
	ret_val.buffIndex = page * SSD1306_PIXELS_X + x;
	ret_val.pageIndex = y - page*SSD1306_PIX_PER_PAGE;
	return ret_val;
}

void SSD1306SendCommand(sSSDdisplay *disp, uint8_t command){
	disp->fpSelectChip();
	disp->fpSetCommand();
	disp->fpSendByte(command);
	disp->fpDeSelectChip();
	return;
}

void SSD1306SendData(sSSDdisplay *disp, uint8_t data){
	disp->fpSelectChip();
	disp->fpSetData();
	disp->fpSendByte(data);
	disp->fpDeSelectChip();
	return;
}
