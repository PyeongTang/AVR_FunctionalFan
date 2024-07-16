/*
 * functionalFan_Fan.h
 *
 * Created: 2024-07-18 오후 2:05:06
 *  Author: Administrator
 */ 


#ifndef FUNCTIONALFAN_FAN_H_
#define FUNCTIONALFAN_FAN_H_

#include "pwm.h"
#include "fnd.h"

#define FAN_STATE_NOSET		0x01
#define FAN_STATE_MODE_1	0x02
#define FAN_STATE_MODE_2	0x04
#define FAN_STATE_MODE_3	0x08

#define FAN_DUTY_NOSET		0x10
#define FAN_DUTY_MODE_1		0xAA
#define FAN_DUTY_MODE_2		0xD4
#define FAN_DUTY_MODE_3		0xFF

#define FAN_POWER_FND_SLIDE_TH_MODE_1	0x0030;
#define FAN_POWER_FND_SLIDE_TH_MODE_2	0x001C;
#define FAN_POWER_FND_SLIDE_TH_MODE_3	0x000C;

FND fnd;
FND* Pfnd;

uint8_t fff_fanState;
uint8_t fff_fanDuty;
uint16_t fff_fndSlideCount;
uint8_t fff_fndSlideSelect;
uint16_t fff_fndSlideThreshold;

void FFF_init();
void FFF_setDuty(uint8_t duty);
void FFF_setState(uint8_t state);
uint8_t FFF_getState ();
void FFF_Execute(bool systemOff);
void FFF_ISR_OC();


#endif /* FUNCTIONALFAN_FAN_H_ */