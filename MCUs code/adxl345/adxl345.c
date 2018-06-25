/*
 * adxl345.c
 *
 *  Created on: 05.06.2018
 *      Author: E6430
 */
#include "adxl345.h"
#include "adxl345_regs.h"

void ADXL_Init(sADXLSPI *adxl, uint8_t (*fcnReadNBytes)(uint8_t, uint8_t, uint8_t *), void (*fcnWriteByte)(uint8_t, uint8_t)){
	adxl->ReadNBytes = fcnReadNBytes;
	adxl->WriteByte = fcnWriteByte;
	adxl->IntDataX[0] = 0;
	adxl->IntDataX[1] = 0;
	adxl->IntDataY[0] = 0;
	adxl->IntDataY[1] = 0;
	adxl->IntDataZ[0] = 0;
	adxl->IntDataZ[1] = 0;
	adxl->AccelerationX = 0;
	adxl->AccelerationY = 0;
	adxl->AccelerationZ = 0;
	adxl->Range = RANGE2G;	// default
}

void ADXL_StartMeasurement(sADXLSPI *adxl){
	// set FULL_RES bit
	ADXL_SetBit(adxl, ADXL_DATA_FORMAT, ADXL_BIT_FULL_RES);
	// set Measure bit
	ADXL_SetBit(adxl, ADXL_POWER_CTL, ADXL_BIT_Measure);
	// also update range value
	adxl->Range = ADXL_GetRange(adxl);
}

uint8_t ADXL_GetReg(sADXLSPI *adxl, uint8_t reg_no){
	uint8_t retval = 0;
	adxl->ReadNBytes(reg_no, 1, &retval);
	return retval;
}

void ADXL_SetReg(sADXLSPI *adxl, uint8_t reg_no, uint8_t reg_value){
	adxl->WriteByte(reg_no, reg_value);
}

void ADXL_SetBit(sADXLSPI *adxl, uint8_t reg_no, uint8_t bit_no){
	// get reg value
	uint8_t reg_value = ADXL_GetReg(adxl, reg_no);
	// set bit
	reg_value |= (1<<bit_no);
	// send to adxl
	ADXL_SetReg(adxl, reg_no, reg_value);
}

void ADXL_ResetBit(sADXLSPI *adxl, uint8_t reg_no, uint8_t bit_no){
	// get reg value
	uint8_t reg_value = ADXL_GetReg(adxl, reg_no);
	// clear bit
	reg_value &= ~(1<<bit_no);
	// send to adxl
	ADXL_SetReg(adxl, reg_no, reg_value);
}

void ADXL_GetDataRegs(sADXLSPI *adxl){
	uint8_t rxbuffer[] = {0,0,0,0,0,0};
	adxl->ReadNBytes(ADXL_DATAX0, 6, rxbuffer);
	adxl->IntDataX[0] = rxbuffer[0];
	adxl->IntDataX[1] = rxbuffer[1];
	adxl->IntDataY[0] = rxbuffer[2];
	adxl->IntDataY[1] = rxbuffer[3];
	adxl->IntDataZ[0] = rxbuffer[4];
	adxl->IntDataZ[1] = rxbuffer[5];

	adxl->AccelerationX = ADXL_2Regs2Float(adxl->IntDataX[1], adxl->IntDataX[0], adxl->Range);
	adxl->AccelerationY = ADXL_2Regs2Float(adxl->IntDataY[1], adxl->IntDataY[0], adxl->Range);
	adxl->AccelerationZ = ADXL_2Regs2Float(adxl->IntDataZ[1], adxl->IntDataZ[0], adxl->Range);
}

tRange ADXL_GetRange(sADXLSPI *adxl){
	// get DATA_FORMAT register
	uint8_t reg_value = ADXL_GetReg(adxl, ADXL_DATA_FORMAT);
	// leave last 2 bits
	reg_value &= 0b00000011;
	return (tRange)reg_value;
}
void ADXL_SetRange(sADXLSPI *adxl, tRange new_range){
	// get DATA_FORMAT register
	uint8_t reg_value = ADXL_GetReg(adxl, ADXL_DATA_FORMAT);
	// set last 2 bit accordingly to new_range
	reg_value &= 0b11111100;
	reg_value |= ((uint8_t)new_range & 0b00000011);
	ADXL_SetReg(adxl, ADXL_DATA_FORMAT, reg_value);
}



float ADXL_2Regs2Float(uint8_t high_byte, uint8_t low_byte, tRange range){
	uint16_t u16_nominator = ((high_byte)<<8 | low_byte);
	int sign = 1;
	int denominator = 1;
	float fullrange = 2;
	switch(range){
		case RANGE2G:		// 10 bit resolution
			//u16_nominator &= 	0b1000001111111111;
			denominator = 	1023;
			fullrange = 2.0;
			break;
		case RANGE4G:		// 11 bit
			//u16_nominator &= 	0b1000011111111111;
			denominator = 	2047;
			fullrange = 4.0;
			break;
		case RANGE8G:		// 12 bit
			//u16_nominator &= 	0b1000111111111111;
			denominator = 	4095;
			fullrange = 8.0;
			break;
		case RANGE16G:		// 13 bit
			//u16_nominator &= 	0b1001111111111111;
			denominator = 	8191;
			fullrange = 16.0;
			break;
		default:
			while(1){}
	}

	if(u16_nominator & 0b1000000000000000){		// sign bit is set - invert value!
		sign = -1;
		u16_nominator = ~u16_nominator +1;
	}

	float retval = ((float)u16_nominator)/((float)denominator);
	retval *= fullrange;
	retval *= sign;
	return retval;
}

float ADXL_GetAccX(sADXLSPI *adxl){
	return adxl->AccelerationX;
}
float ADXL_GetAccY(sADXLSPI *adxl){
	return adxl->AccelerationY;
}
float ADXL_GetAccZ(sADXLSPI *adxl){
	return adxl->AccelerationZ;
}
