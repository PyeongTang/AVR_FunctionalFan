/*
 * pwm.c
 *
 * Created: 2024-06-25 오후 4:33:24
 *  Author: Lee JaePyeong
 */ 
// 
// 

// --------------------------
// Timer/Counter0			|
// --------------------------
//  		SINGLE			|
// --------------------------
//			PB4				|
//			OC0				|
// --------------------------

// --------------------------
// Timer/Counter1			|
// --------------------------
//  CH_A	CH_B	CH_C	|
// --------------------------
//	PB5		PB6		PB7		|
//	OC1A	OC1B	OC1C	|
// --------------------------

// --------------------------
// Timer/Counter2			|
// --------------------------
//  		SINGLE			|
// --------------------------
//			PB7				|
//			OC2				|
// --------------------------

// --------------------------
// Timer/Counter3			|
// --------------------------
//  CH_A	CH_B	CH_C	|
// --------------------------
//	PE3		PE4		PE5		|
//	OC3A	OC3B	OC3C	|
// --------------------------

#include "pwm.h"

void PWM_init (uint8_t timerNum, char channel, uint16_t desiredFreqency){
	switch (timerNum){
		case (TIMER_0) :
				TIMER0_init();
				TIMER0_setPwm();
		break;
			
		case (TIMER_1) :
			switch (channel) {
				case ('A') :
					TIMER1_init			(TIMER1_CH_A_PORT);
					TIMER1_setPwm		(TIMER1_CH_A, desiredFreqency);
				break;
				
				case ('a') :
					TIMER1_init			(TIMER1_CH_A_PORT);
					TIMER1_setPwm		(TIMER1_CH_A, desiredFreqency);
				break;
				
				case ('B') :
					TIMER1_init			(TIMER1_CH_B_PORT);
					TIMER1_setPwm		(TIMER1_CH_B, desiredFreqency);
				break;
				
				case ('b') :
					TIMER1_init			(TIMER1_CH_B_PORT);
					TIMER1_setPwm		(TIMER1_CH_B, desiredFreqency);
				break;
				
				case ('C') :
					TIMER1_init			(TIMER1_CH_C_PORT);
					TIMER1_setPwm		(TIMER1_CH_C, desiredFreqency);
				break;
				
				case ('c') :
					TIMER1_init			(TIMER1_CH_C_PORT);
					TIMER1_setPwm		(TIMER1_CH_C, desiredFreqency);
				break;
			}
		break;
		
		case (TIMER_2) :
			TIMER2_init();
			TIMER2_setPwm();
		break;
		
		case (TIMER_3) :
			switch (channel) {
				case ('A') :
					TIMER3_init			(TIMER3_CH_A_PORT);
					TIMER3_setPwm		(TIMER3_CH_A, desiredFreqency);
				break;
				
				case ('a') :
					TIMER3_init			(TIMER3_CH_A_PORT);
					TIMER3_setPwm		(TIMER3_CH_A, desiredFreqency);
				break;
				
				case ('B') :
					TIMER3_init			(TIMER3_CH_B_PORT);
					TIMER3_setPwm		(TIMER3_CH_B, desiredFreqency);
				break;
				
				case ('b') :
					TIMER3_init			(TIMER3_CH_B_PORT);
					TIMER3_setPwm		(TIMER3_CH_B, desiredFreqency);
				break;
				
				case ('C') :
					TIMER3_init			(TIMER3_CH_C_PORT);
					TIMER3_setPwm		(TIMER3_CH_C, desiredFreqency);
				break;
				
				case ('c') :
					TIMER3_init			(TIMER3_CH_C_PORT);
					TIMER3_setPwm		(TIMER3_CH_C, desiredFreqency);
				break;
			}
		break;
	}
}
void PWM_setDuty (uint8_t timerNum, char channel, uint16_t stepValue){
	switch (timerNum){
		case (TIMER_0) :
			TIMER0_setDuty(stepValue);
		break;
			
		case (TIMER_1) :
			switch (channel) {
				case ('A') :
					TIMER1_setDuty(TIMER1_CH_A, stepValue);
				break;
				
				case ('a') :
					TIMER1_setDuty(TIMER1_CH_A, stepValue);
				break;
				
				case ('B') :
					TIMER1_setDuty(TIMER1_CH_B, stepValue);
				break;
				
				case ('b') :
					TIMER1_setDuty(TIMER1_CH_B, stepValue);
				break;
				
				case ('C') :
					TIMER1_setDuty(TIMER1_CH_C, stepValue);
				break;
				
				case ('c') :
					TIMER1_setDuty(TIMER1_CH_C, stepValue);
				break;
			}
		break;
		
		case (TIMER_2) :
			TIMER2_setDuty(stepValue);
		break;
		
		case (TIMER_3) :
			switch (channel) {
				case ('A') :
					TIMER3_setDuty(TIMER3_CH_A, stepValue);
				break;
				
				case ('a') :
					TIMER3_setDuty(TIMER3_CH_A, stepValue);
				break;
				
				case ('B') :
					TIMER3_setDuty(TIMER3_CH_B, stepValue);
				break;
				
				case ('b') :
					TIMER3_setDuty(TIMER3_CH_B, stepValue);
				break;
				
				case ('C') :
					TIMER3_setDuty(TIMER3_CH_C, stepValue);
				break;
				
				case ('c') :
					TIMER3_setDuty(TIMER3_CH_C, stepValue);
				break;
			}
		break;
	}
}