/*
 * sg90.c
 *
 * Created: 2024-07-17 오후 5:07:18
 *  Author: Lee JaePyeong
 */ 

#include "sg90.h"

void SG90_init (SERVO* servo, uint8_t timerNumber, char channel){
	servo->timerNumber = timerNumber;
	servo->timerChannel = channel;
	PWM_init(timerNumber, channel, SERVO_PWM_FREQ);
}

void SG90_setAngle (SERVO* servo, uint8_t angle){
	servo->angle = angle;
	PWM_setDuty(servo->timerNumber, servo->timerChannel, SG90_angle2Step(servo, angle));
}

uint16_t SG90_angle2Step(SERVO* servo, uint8_t angle){
// 	0 <= angle <= 180, where angle is integer
	servo->dutyStep = (uint16_t)(angle*SERVO_UNIT_STEP + SERVO_STEP_MIN);
	return servo->dutyStep;
}

uint8_t SG90_getAngle(SERVO* servo){
	return servo->angle;
}

uint16_t SG90_getDutyStep(SERVO* servo){
	return servo->dutyStep;
}