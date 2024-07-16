/*
 * functionalFan_BackLight.h
 *
 * Created: 2024-07-17 오전 11:23:52
 *  Author: Lee JaePyeong
 */ 


#ifndef FUNCTIONALFAN_BACKLIGHT_H_
#define FUNCTIONALFAN_BACKLIGHT_H_

#include "adc.h"
#include "pwm.h"

// Using Timer 1 CH B

#define BACKLIGHT_DDR		DDRB
#define BACKLIGHT_PORT		PORTB
#define BACKLIGHT_LED		6
#define BACKLIGHT_PWM_FREQ	10000

uint8_t FFB_backLightDuty;

void FFB_init();
void FFB_ISR ();
void FFB_Execute (bool systemOff);
void FFB_setDuty (uint16_t dutyValue);
uint8_t FFB_getDutyValue ();

#endif /* FUNCTIONALFAN_BACKLIGHT_H_ */