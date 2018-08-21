/*
 * ledblinker.h
 *
 *  Created on: 20.08.2018
 *      Author: E6430
 */

#ifndef LEDBLINKER_LEDBLINKER_H_
#define LEDBLINKER_LEDBLINKER_H_

#define LEDPULSE_SHORT_TIME 	10		// duration of short pulse of signal in [SysTicks], when counting pulses
#define LEDPULSE_LONG_TIME		20		// duration of long pulse of signal in [SysTicks], when counting pulses
#define LEDBLINKING_SHORT_TIME	10		// duration of short pulse of signal in [SysTicks], when blinking
#define LEDBLINKING_LONG_TIME	40		// duration of long pulse of signal in [SysTicks], when blinking

typedef enum {	MODE_OFF		= 0x00000000,
				MODE_ON			= 0x00000001,
				MODE_BLINKING	= 0x00000002,
				MODE_BLINK_N	= 0x00000004
}eBlinkingMode;

typedef enum {	STATE_LED_OFF,
				STATE_LED_OFF_ENTRY,
				STATE_LED_ON,
				STATE_LED_ON_ENTRY
}eLedState;

typedef struct{
	eLedState		LEDState;
	eBlinkingMode 	LEDMode;
	int 			LedTimer;
	int 			BlinkCounter;
	int				Time_ON;				// ON state duration in [SysTicks]
	int				Time_OFF;				// OFF state duration in [SysTicks]
	void 			(*fLedOn)(void);
	void 			(*fLedOff)(void);
}sLEDInstance;

void LEDInstanceInit(sLEDInstance *, void(*)(), void(*)());
void LEDInstanceProcess(sLEDInstance *);
void LEDInstanceSetState(sLEDInstance *, eBlinkingMode, int, int, int);
void LEDInstanceQuickBlink(sLEDInstance *, int);
void LEDInstanceSetBlinkingLowDuty(sLEDInstance *);
void LEDInstanceSetBlinkingHighDuty(sLEDInstance *);
void LEDInstanceSetOFF(sLEDInstance *);
void LEDInstanceSetON(sLEDInstance *);

#endif /* LEDBLINKER_LEDBLINKER_H_ */
