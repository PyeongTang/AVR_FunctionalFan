/*
 * pwm.h
 *
 * Created: 2024-06-25 오후 4:33:35
 *  Author: Lee JaePyeong
 */ 


#ifndef PWM_H_
#define PWM_H_

#include "def.h"
#include "timer0.h"
#include "timer2.h"
#include "timer1.h"
#include "timer3.h"

#define TIMER_0				0
#define TIMER_1				1
#define TIMER_2				2
#define TIMER_3				3

void PWM_init (uint8_t timerNum, char channel, uint16_t desiredFreqency);
void PWM_setDuty (uint8_t timerNum, char channel, uint16_t stepValue);

#endif /* PWM_H_ */