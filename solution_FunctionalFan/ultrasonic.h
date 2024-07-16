/*
 * ultrasonic.h
 *
 * Created: 2024-06-24 오후 12:20:11
 *  Author: Lee JaePyeong
 */ 


#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_

#include "def.h"
#include "pwm.h"

// Output PWM
#define ULTRASONIC_TRIG_DDR			DDRB
#define ULTRASONIC_TRIG_PORT		PORTB
#define ULTRASONIC_TRIG_PORT_NUM	PORTB6

// Input Capture
#define ULTRASONIC_ECHO_DDR			DDRD
#define ULTRASONIC_EHCO_PIN			PIND
#define ULTRASONIC_ECHO_PIN_NUM		PIND4

#define ULTRASONIC_PRESCALER		64
#define ULTRASONIC_TRIG_PWM_FREQ	20
#define ULTRASONIC_TRIG_FULL_STEP	F_CPU / ULTRASONIC_PRESCALER / ULTRASONIC_TRIG_PWM_FREQ
#define ULTRASONIC_TRIG_HIGH_STEP	ULTRASONIC_TRIG_FULL_STEP / 5000

volatile uint8_t	ULTRA_isDataCaptured;
volatile uint16_t	ULTRA_distance;
volatile uint16_t	ULTRA_startCount;
volatile uint16_t	ULTRA_endCount;
volatile uint16_t	tempCount;

void ULTRA_init ();
void ULTRA_inputCaptureInit ();
void ULTRA_enableTrigger ();
void ULTRA_ISR_CAPT ();
uint16_t ULTRA_EXE_getDistance ();
uint16_t ULTRA_determineDistance();

#endif /* ULTRASONIC_H_ */