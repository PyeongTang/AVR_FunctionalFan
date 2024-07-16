/*
 * functionalFan_Timer.h
 *
 * Created: 2024-07-17 오전 9:13:11
 *  Author: Lee JaePyeong
 */ 


#ifndef FUNCTIONALFAN_TIMER_H_
#define FUNCTIONALFAN_TIMER_H_

#include "timer0.h"
#include "fnd4.h"

// Using Timer 0

#define	TIMER_STATE_NOSET		0x01
#define TIMER_STATE_MODE_1		0x02
#define TIMER_STATE_MODE_2		0x04
#define TIMER_STATE_MODE_3		0x08

#define TIMER_TH_MODE_1			10		// 10 sec
#define TIMER_TH_MODE_2			60		// 1 min
#define TIMER_TH_MODE_3			600		// 10 min

volatile uint8_t				fft_timerState;
volatile uint16_t				fft_timerThreshold;
volatile uint16_t				fft_timerTickValue;
bool							fft_timeOver;

FND4							fnd4;
FND4*							Pfnd4;

void FFT_ISR ();
void FFT_init ();
void FFT_setState (uint8_t state);
bool FFT_isTimeOver();
uint8_t FFT_getState ();
uint16_t FFT_getTimeValue();
void FFT_Execute (bool systemOff);

#endif /* FUNCTIONALFAN_TIMER_H_ */