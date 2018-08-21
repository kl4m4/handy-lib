/*
 * ledblinker.c
 *
 *  Created on: 20.08.2018
 *      Author: E6430
 */

#include "ledblinker.h"


void LEDInstanceInit(sLEDInstance *led, void(*fLedOn)(), void(*fLedOff)()){
	led->BlinkCounter = 0;
	led->LEDMode = MODE_OFF;
	led->LEDState = STATE_LED_OFF;
	led->LedTimer = 0;
	led->fLedOn = fLedOn;
	led->fLedOff = fLedOff;
	led->fLedOff();
}


void LEDInstanceSetState(sLEDInstance *led, eBlinkingMode mode, int T_On, int T_Off, int Counter){
	led->BlinkCounter = Counter;
	if(T_On < 2){
		T_On = 2;
	}
	if(T_Off < 2){
		T_Off = 2;
	}
	led->Time_OFF = T_Off - 2;		// -2 because counting 9...0, not 10...1, also there is 1 Process cycle to switch between states
	led->Time_ON = T_On - 2;
	led->LEDMode = mode;
	if(mode != MODE_OFF){
		led->LEDState = STATE_LED_ON_ENTRY;
	}else{
		led->LEDState = STATE_LED_OFF_ENTRY;
	}
}


void LEDInstanceProcess(sLEDInstance *led){
	eLedState NextState = led->LEDState;
	switch(led->LEDState){
		case STATE_LED_OFF:
			if(led->LEDMode == MODE_ON){
				NextState = STATE_LED_ON_ENTRY;
			}
			if(led->LEDMode & (MODE_BLINKING | MODE_BLINK_N)){
				if(led->LedTimer <= 0){
					NextState = STATE_LED_ON_ENTRY;
				}else{
					led->LedTimer--;
				}
			}
			break;
		case STATE_LED_ON:
			if(led->LEDMode == MODE_OFF){
				NextState = STATE_LED_OFF_ENTRY;
			}
			if(led->LEDMode & (MODE_BLINKING | MODE_BLINK_N)){
				if(led->LedTimer <= 0){
					NextState = STATE_LED_OFF_ENTRY;
				}else{
					led->LedTimer--;
				}
			}
			break;
		case STATE_LED_OFF_ENTRY:
			led->fLedOff();
			NextState = STATE_LED_OFF;
			led->LedTimer = led->Time_OFF;
			break;
		case STATE_LED_ON_ENTRY:
			if(led->LEDMode == MODE_BLINK_N){
				if(led->BlinkCounter>0){
					led->BlinkCounter--;
					led->fLedOn();
					NextState = STATE_LED_ON;
					led->LedTimer = led->Time_ON;
				}else{
					led->BlinkCounter = 0;
					NextState = STATE_LED_OFF_ENTRY;
					led->LEDMode = MODE_OFF;
				}
			}else{
				led->fLedOn();
				NextState = STATE_LED_ON;
				led->LedTimer = led->Time_ON;
			}

			break;
	}
	led->LEDState = NextState;
}

void LEDInstanceQuickBlink(sLEDInstance *led, int counter){
	LEDInstanceSetState(led, MODE_BLINK_N, LEDPULSE_SHORT_TIME, LEDPULSE_LONG_TIME, counter);
}

void LEDInstanceSetBlinkingLowDuty(sLEDInstance *led){
	LEDInstanceSetState(led, MODE_BLINKING, LEDBLINKING_SHORT_TIME, LEDBLINKING_LONG_TIME, 0);
}

void LEDInstanceSetBlinkingHighDuty(sLEDInstance *led){
	LEDInstanceSetState(led, MODE_BLINKING, LEDBLINKING_LONG_TIME, LEDBLINKING_SHORT_TIME, 0);
}

void LEDInstanceSetOFF(sLEDInstance *led){
	LEDInstanceSetState(led, MODE_OFF, 0, 0, 0);
}
void LEDInstanceSetON(sLEDInstance *led){
	LEDInstanceSetState(led, MODE_ON, 0, 0, 0);
}
