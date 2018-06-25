/*
 * adxl345.h
 *
 *  Created on: 03.06.2018
 *      Author: E6430
 */

#ifndef ADXL345_ADXL345_H_
#define ADXL345_ADXL345_H_

#include <inttypes.h>


///
///	\brief Typedef for storing measurement range value
///
typedef enum{
	RANGE2G 	= 0,
	RANGE4G 	= 1,
	RANGE8G 	= 2,
	RANGE16G 	= 3
}tRange;

///
/// \brief Typedef structure for storing accelerometer data and functions
/// Stores data retrieved from device, and pointers to functions used as a interface to SPI hardware. This part should be platform independent.
///
typedef struct {
	// Interface functions, to be assigned in init:
	void (*WriteByte)(uint8_t, uint8_t);					///< Provides SPI interface for transmiting data
	uint8_t (*ReadNBytes)(uint8_t, uint8_t, uint8_t *);		///< Provides SPI interface for receiving data
	// Data registers
	uint8_t IntDataX[2];									///< Holds measured data for X axis in 2 bytes array
	uint8_t IntDataY[2];									///< Holds measured data for Y axis in 2 bytes array
	uint8_t IntDataZ[2];									///< Holds measured data for Z axis in 2 bytes array
	float AccelerationX;									///< Holds measured data for X axis in float
	float AccelerationY;									///< Holds measured data for Y axis in float
	float AccelerationZ;									///< Holds measured data for Z axis in float
	tRange Range;											///< Holds measurement range setting
}sADXLSPI;



// external interface

///
/// \brief Initializes structure representing ADXL device
///
void ADXL_Init(sADXLSPI *, uint8_t (*)(uint8_t, uint8_t, uint8_t *), void (*)(uint8_t, uint8_t));

///
/// \brief Puts device in measurement mode, also set full resolution mode
///
void ADXL_StartMeasurement(sADXLSPI *);

///
/// \brief Returns range settings from ADXL device
///
tRange ADXL_GetRange(sADXLSPI *);

///
/// \brief Sets range settings in ADXL device
///
void ADXL_SetRange(sADXLSPI *, tRange);

///
/// \brief Returns last X axis measurement read in float format
///
float ADXL_GetAccX(sADXLSPI *);

///
/// \brief Returns last Y axis measurement read in float format
///
float ADXL_GetAccY(sADXLSPI *);

///
/// \brief Returns last Z axis measurement read in float format
///
float ADXL_GetAccZ(sADXLSPI *);


// internal functions
float ADXL_2Regs2Float(uint8_t, uint8_t, tRange);
uint8_t ADXL_GetReg(sADXLSPI *, uint8_t);
void ADXL_SetReg(sADXLSPI *, uint8_t, uint8_t);
void ADXL_SetBit(sADXLSPI *, uint8_t, uint8_t);
void ADXL_ResetBit(sADXLSPI *, uint8_t, uint8_t);
void ADXL_GetDataRegs(sADXLSPI *);

#endif /* ADXL345_ADXL345_H_ */
