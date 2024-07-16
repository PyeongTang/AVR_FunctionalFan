/*
 * timer2.h
 *
 * Created: 2024-07-18 오후 12:22:22
 *  Author: Lee JaePyeong
 */ 


#ifndef TIMER2_H_
#define TIMER2_H_

#include "def.h"

#define TIMER2_OC_DDR						DDRB
#define TIMER2_OC_PORT						PORTB

#define TIMER2_OC_PORT_NUM					PORTB7
#define TIMER2_MILLI_OCR0_VALUE				249
#define TIMER2_MILLI_TCNT0_VALUE			7

// CLOCK_MODE_SET
#define	TIMER2_CLOCK_SELECT_NO_SOURCE		0x00
#define	TIMER2_CLOCK_SELECT_PRE_1			0x01
#define	TIMER2_CLOCK_SELECT_PRE_8			0x02
#define	TIMER2_CLOCK_SELECT_PRE_32			0x03
#define	TIMER2_CLOCK_SELECT_PRE_64			0x04
#define	TIMER2_CLOCK_SELECT_PRE_128			0x05
#define	TIMER2_CLOCK_SELECT_PRE_256			0x06
#define	TIMER2_CLOCK_SELECT_PRE_1024		0x07

// COM_MODE_SET
#define TIMER2_COM_MODE_NORMAL				0x00
#define TIMER2_COM_MODE_TOGGLE				0x01
#define TIMER2_COM_MODE_NON_INVERT			0x02
#define TIMER2_COM_MODE_INVERT				0x03

// WGM_MODE_SET
#define TIMER2_WGM_MODE_NORMAL				0x00
#define TIMER2_WGM_MODE_PC_PWM				0x01
#define TIMER2_WGM_MODE_CTC					0x02
#define TIMER2_WGM_MODE_F_PWM				0x03

volatile uint16_t TIMER2_secondPassed;
volatile uint32_t TIMER2_tickCount1ms;

// Initialization
void TIMER2_init ();

// PWM
void TIMER2_setPwm ();
void TIMER2_setDuty (uint8_t stepValue);

// TICK
void TIMER2_enableTick1ms ();
void TIMER2_ISR_OVF_tick1ms();
void TIMER2_ISR_COMP_tick1ms ();

// Set Wave Generation Mode
void TIMER2_setWaveGenerationMode (uint8_t modeNum);

// Set Compare Output Mode
void TIMER2_setCompareOutputMode (uint8_t modeNum);

// Set Clock Mode
void TIMER2_setClockMode (uint8_t modeNum);

// Enable Force Output Compare
void TIMER2_enableForceOutputCompare ();

// Enable Output Compare Interrupt
void TIMER2_enableOutputCompareInterrupt ();

// Disable Output Comapre Interrupt
void TIMER2_disableOutputCompareInterrupt ();

// Enable Overflow Interrupt
void TIMER2_enableOverflowInterrupt ();

// Disable Overflow Interrupt
void TIMER2_disableOverflowInterrupt ();

// Output Compare Register Set
void TIMER2_setOutputCompareReg (uint8_t outputCompareThreshold);

// Timer/Counter Register Set
void TIMER2_setTimerCounterReg (uint8_t count);

// Time Functions
uint32_t TIMER2_millis ();
uint16_t TIMER2_seconds();

#endif /* TIMER2_H_ */