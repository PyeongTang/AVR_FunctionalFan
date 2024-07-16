/*
 * functionalFan_BackLight.c
 *
 * Created: 2024-07-17 오전 11:23:34
 *  Author: Lee JaePyeong
 */ 

#include "functionalFan_BackLight.h"

void FFB_init(){
	BACKLIGHT_DDR	|= (1 << BACKLIGHT_LED);
	BACKLIGHT_PORT	&= ~(1 << BACKLIGHT_LED);
	FFB_backLightDuty = 0x00;
	ADC_init(ADC_CH_0);
	PWM_init(TIMER_1, TIMER1_CH_B, BACKLIGHT_PWM_FREQ);
}

void FFB_ISR (){
	FFB_setDuty (ADC);
}

void FFB_Execute (bool systemOff){
	if (systemOff) {
		ADC_disableADC();
		FFB_backLightDuty = 0x00;
		PWM_setDuty(TIMER_1, TIMER1_CH_B, FFB_backLightDuty);
	}
	else {
		ADC_EXE_freeRunning();
	}
}

void FFB_setDuty (uint16_t dutyValue){
	FFB_backLightDuty = 100.0 * (float)(dutyValue)/1024.0;
	PWM_setDuty(TIMER_1, TIMER1_CH_B, FFB_backLightDuty);
}

uint8_t FFB_getDutyValue (){
	return FFB_backLightDuty;
}