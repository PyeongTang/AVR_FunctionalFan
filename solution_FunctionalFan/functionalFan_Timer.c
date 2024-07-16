/*
 * functionalFan_Timer.c
 *
 * Created: 2024-07-17 오전 9:12:46
 *  Author: Lee JaePyeong
 */ 

#include "functionalFan_Timer.h"

void FFT_ISR (){
	TIMER0_ISR_OVF_tick1ms();
	if (~fft_timeOver){
		fft_timerTickValue = fft_timerThreshold - TIMER0_seconds();
		if (fft_timerTickValue <= 0) {
			fft_timeOver = true;
		}
	}
}

void FFT_init (){
	fft_timerState		=	TIMER_STATE_NOSET;
	fft_timeOver		=	false;
	fft_timerTickValue  =	0;
	fft_timerThreshold	=	0;
	TIMER0_init();
	Pfnd4 = &(fnd4);
	FND4_init(Pfnd4);
	FND4_allOff(Pfnd4);
}

void FFT_setState (uint8_t state){
	TIMER0_disableOverflowInterrupt();
	fft_timerState		= state;
	fft_timeOver		= false;
	fft_timerTickValue  = 0;
}

uint8_t FFT_getState (){
	return fft_timerState;
}

uint16_t FFT_getTimeValue() {
	return fft_timerTickValue;
}

bool FFT_isTimeOver(){
	return fft_timeOver;
}

void FFT_Execute (bool systemOff){
// 	Reset
	if (systemOff || fft_timeOver) {
		fft_timerState			=	TIMER_STATE_NOSET;
		fft_timeOver			=	false;
		TIMER0_disableOverflowInterrupt();
		FND4_allOff(Pfnd4);
	}
// 	Determine Threshold for Timer
	else {
		if		(fft_timerState == TIMER_STATE_NOSET) {
			fft_timerThreshold	= TIMER_TH_MODE_1;
			TIMER0_disableOverflowInterrupt();
		}
		else if (fft_timerState == TIMER_STATE_MODE_1) {
			fft_timerThreshold	= TIMER_TH_MODE_1;
			TIMER0_enableOverflowInterrupt();
		}
		else if (fft_timerState == TIMER_STATE_MODE_2) {
			fft_timerThreshold	= TIMER_TH_MODE_2;
			TIMER0_enableOverflowInterrupt();
		}
		else if (fft_timerState == TIMER_STATE_MODE_3) {
			fft_timerThreshold	= TIMER_TH_MODE_3;
			TIMER0_enableOverflowInterrupt();
		}
		FND4_setValue(Pfnd4, FFT_getTimeValue());
	}
}