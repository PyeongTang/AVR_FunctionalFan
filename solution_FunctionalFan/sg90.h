/*
 * sg90.h
 *
 * Created: 2024-07-17 오후 5:07:26
 *  Author: Lee JaePyeong
 */ 


#ifndef SG90_H_
#define SG90_H_

#include "pwm.h"

#define SERVO_PWM_FREQ		50
#define SERVO_STEP_MIN		1000 // 1,000 / 40,000 steps
#define SERVO_STEP_MAX		5000 // 5,000 / 40,000 steps
#define SERVO_UNIT_STEP		22   // (5,000 - 1,000) / 180, 22.2 steps

typedef struct {
	uint8_t timerNumber;
	char timerChannel;
	uint16_t dutyStep;
	uint8_t angle;
} SERVO;

void SG90_init (SERVO* servo, uint8_t timerNumber, char channel);
void SG90_setAngle (SERVO* servo, uint8_t angle);
void SG90_setStep (SERVO* servo, uint16_t step);
uint16_t SG90_angle2Step(SERVO* servo, uint8_t angle);
uint8_t SG90_getAngle(SERVO* servo);
uint16_t SG90_getDutyStep(SERVO* servo);

#endif /* SG90_H_ */