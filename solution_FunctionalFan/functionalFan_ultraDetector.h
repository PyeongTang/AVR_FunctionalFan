/*
 * ultraDetector.h
 *
 * Created: 2024-07-17 오후 9:57:15
 *  Author: Lee JaePyeong
 */ 


#ifndef FUNCTIONALFAN_ULTRADETECTOR_H_
#define FUNCTIONALFAN_ULTRADETECTOR_H_

#include "sg90.h"
#include "ultrasonic.h"

#define SERVO_WIPER_ANGLE_MAX	180
#define SERVO_WIPER_ANGLE_MIN	0

#define ULTRA_DETECT_TH_CM		5

// Using Timer/Counter 1 CH A, B for Ultrasonic sensor
// 	ECHO : PD4 (ICP1)
//	TRIG : PB6 (OC1B)

// Using Timer/Counter 3 CH A for Servo Pointer
//	DATA : PE3 (OC3A)

// Using Timer/Counter 3 CH B for Servo Wiper
//	DATA : PE4 (OC3B)

// Using Timer/Counter 3 CH C for Servo Wiping Controller
//		-: PE5 (OC3C)

SERVO	UDWiper;
SERVO*	PUDWiper;

SERVO	UDPointer;
SERVO*	PUDPointer;

volatile uint8_t servoWiperAngle;
volatile uint8_t servoWiperCount;
volatile bool servoWiperIncrease;

void UD_COMP_ISR ();
void UD_init ();
void UD_Execute(bool systemOff);
void UD_pointServoHead(SERVO* servo, uint8_t pointingAngle);
uint8_t UD_getWiperAngle();

#endif /* FUNCTIONALFAN_ULTRADETECTOR_H_ */