/*
 * functionalFan_Fan.c
 *
 * Created: 2024-07-18 오후 2:04:39
 *  Author: Administrator
 */ 

#include "functionalFan_Fan.h"

void FFF_init(){
	fff_fanState = FAN_STATE_NOSET;
	Pfnd = &(fnd);
	fff_fanDuty = 0x00;
	fff_fndSlideCount = 0;
	fff_fndSlideThreshold = 0;
	fff_fndSlideSelect = 0;
	PWM_init(TIMER_2, 'a', 1);
	TIMER2_enableOutputCompareInterrupt();
	FND_init(Pfnd);
	FND_allOff(Pfnd);
};

void FFF_setDuty(uint8_t duty){
	PWM_setDuty(TIMER_2, 'a', duty);
}

// BTN1 (INT1)
void FFF_setState(uint8_t state){
	fff_fanState = state;
}

uint8_t FFF_getState (){
	return fff_fanState;
}

// while(1)
void FFF_Execute(bool systemOff){
	// 	Reset (TimeOut or ButtonActivated)
	if (systemOff) {
		fff_fanState			=	FAN_STATE_NOSET;
		fff_fanDuty				=	FAN_DUTY_NOSET;
		FFF_setDuty(fff_fanDuty);
		FND_middleBar(Pfnd);
		TIMER2_disableOutputCompareInterrupt();
	}
	// 	Determine Duty for Fan, DC Motor
	else {
		if		(fff_fanState == FAN_STATE_NOSET) {
			fff_fanDuty = FAN_DUTY_NOSET;
			FFF_setDuty(fff_fanDuty);
			FND_middleBar(Pfnd);
			TIMER2_disableOutputCompareInterrupt();
		}
		else if (fff_fanState == FAN_STATE_MODE_1) {
			fff_fanDuty = FAN_DUTY_MODE_1;
			FFF_setDuty(fff_fanDuty);
			TIMER2_enableOutputCompareInterrupt();
		}
		else if (fff_fanState == FAN_STATE_MODE_2) {
			fff_fanDuty = FAN_DUTY_MODE_2;
			FFF_setDuty(fff_fanDuty);
			TIMER2_enableOutputCompareInterrupt();
		}
		else if (fff_fanState == FAN_STATE_MODE_3) {
			fff_fanDuty = FAN_DUTY_MODE_3;
			FFF_setDuty(fff_fanDuty);
			TIMER2_enableOutputCompareInterrupt();
		}
	}
	
}

// TIMER2_OC_vect
void FFF_ISR_OC(){
	switch (fff_fanState){
		case FAN_STATE_NOSET  : break;
		
		case FAN_STATE_MODE_1 :
			fff_fndSlideThreshold = FAN_POWER_FND_SLIDE_TH_MODE_1;
		break;
		
		case FAN_STATE_MODE_2 :
			fff_fndSlideThreshold = FAN_POWER_FND_SLIDE_TH_MODE_2;
		break;
		
		case FAN_STATE_MODE_3 :
			fff_fndSlideThreshold = FAN_POWER_FND_SLIDE_TH_MODE_3;
		break;
	}
	if (fff_fanState != FAN_STATE_NOSET){
		fff_fndSlideCount++;
		if (fff_fndSlideCount >= fff_fndSlideThreshold){
			fff_fndSlideCount = 0;
			if (fff_fndSlideSelect >= 5){
				fff_fndSlideSelect = 0;
			}
			else {
				fff_fndSlideSelect++;
			}
			FND_slide(Pfnd, fff_fndSlideSelect);
		}
	}
	else {
		FND_middleBar(Pfnd);
	}
}
