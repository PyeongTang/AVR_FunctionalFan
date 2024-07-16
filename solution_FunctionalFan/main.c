/*
 * solution_FunctionalFan.c
 *
 * Created: 2024-07-16 오후 6:27:14
 * Author : Lee JaePyeong
 */ 


// Using Timer/Counter 1 CH A, B for Ultrasonic sensor
// 	ECHO : PD4 (ICP1)
//	TRIG : PB6 (OC1B)

// Using Timer/Counter 3 CH A for Servo Pointer
//	DATA : PE3 (OC3A)

// Using Timer/Counter 3 CH B for Servo Wiper
//	DATA : PE4 (OC3B)

// Using Timer/Counter 3 CH C for Servo Wiping Controller
//		-: PE5 (OC3C)

#include "functionalFan_Timer.h"
#include "functionalFan_ultraDetector.h"
#include "functionalFan_Fan.h"
#include "button.h"

bool	systemDisable;

BTN		btnTimerControl;
BTN*	PbtnTimerControl	= &(btnTimerControl);

BTN		btnFanControl;
BTN*	PbtnFanControl		= &(btnFanControl);

BTN		btnSystemControl;
BTN*	PbtnSystemControl	= &(btnSystemControl);

// Button to System Control
ISR (INT2_vect){
	if (systemDisable) {
		systemDisable = false;
	}
	else {
		systemDisable = true;
	}
}

// Timer
ISR (TIMER0_OVF_vect){
	FFT_ISR();
}

// Button to Timer Control
ISR (INT1_vect){
	switch (FFT_getState()) {
		
		case TIMER_STATE_NOSET :
			FFT_setState(TIMER_STATE_MODE_1);
		break;
		
		case TIMER_STATE_MODE_1 :
			FFT_setState(TIMER_STATE_MODE_2);
		break;
		
		case TIMER_STATE_MODE_2 :
			FFT_setState(TIMER_STATE_MODE_3);
		break;
		
		case TIMER_STATE_MODE_3 :
			FFT_setState(TIMER_STATE_NOSET);
		break;
		
		default:break;
	}
}

// Button to DC Motor Fan Power ControlSS
ISR (INT0_vect){
	switch (FFF_getState()) {
		
		case FAN_STATE_NOSET :
			FFF_setState(FAN_STATE_MODE_1);
		break;
		
		case FAN_STATE_MODE_1 :
			FFF_setState(FAN_STATE_MODE_2);
		break;
		
		case FAN_STATE_MODE_2 :
			FFF_setState(FAN_STATE_MODE_3);
		break;
		
		case FAN_STATE_MODE_3 :
			FFF_setState(FAN_STATE_NOSET);
		break;
		
		default:break;
	}
}

// UltraDetector
ISR (TIMER3_COMPC_vect){
	UD_COMP_ISR();
}

// Ultrasonic
ISR (TIMER1_CAPT_vect){
	ULTRA_ISR_CAPT();
}

// Fan (DC Motor)
ISR (TIMER2_COMP_vect){
	FFF_ISR_OC();
}

int main(void)
{
	systemDisable = true;
	
	BTN_init(PbtnTimerControl,		BTN_0);
	BTN_enableInterrupt(PbtnTimerControl);
	
	BTN_init(PbtnFanControl,		BTN_1);
	BTN_enableInterrupt(PbtnFanControl);
	
	BTN_init(PbtnSystemControl,		BTN_2);
	BTN_enableInterrupt(PbtnSystemControl);
	
	FFT_init	();
	UD_init		();
	FFF_init	();

	while (1) {
		FFT_Execute	(systemDisable);
		UD_Execute	(systemDisable);
		FFF_Execute	(systemDisable);
		
		if (~systemDisable && FFT_isTimeOver()){
			systemDisable = true;
		}
	}
}

