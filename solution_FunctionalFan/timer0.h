/*
 * timer0.h
 *
 * Created: 2024-06-17 오전 8:48:08
 *  Author: Lee JaePyeong
 */ 


#ifndef TIMER0_H_
#define TIMER0_H_

#include "def.h"

#define TIMER0_OC_DDR						DDRB
#define TIMER0_OC_PORT						PORTB

#define TIMER0_OC_PORT_NUM					PORTB4
#define TIMER0_MILLI_OCR0_VALUE				249
#define TIMER0_MILLI_TCNT0_VALUE			7

// CLOCK_MODE_SET
#define	TIMER0_CLOCK_SELECT_NO_SOURCE		0x00
#define	TIMER0_CLOCK_SELECT_PRE_1			0x01
#define	TIMER0_CLOCK_SELECT_PRE_8			0x02
#define	TIMER0_CLOCK_SELECT_PRE_32			0x03
#define	TIMER0_CLOCK_SELECT_PRE_64			0x04
#define	TIMER0_CLOCK_SELECT_PRE_128			0x05
#define	TIMER0_CLOCK_SELECT_PRE_256			0x06
#define	TIMER0_CLOCK_SELECT_PRE_1024		0x07

// COM_MODE_SET
#define TIMER0_COM_MODE_NORMAL				0x00
#define TIMER0_COM_MODE_TOGGLE				0x01
#define TIMER0_COM_MODE_NON_INVERT			0x02
#define TIMER0_COM_MODE_INVERT				0x03

// WGM_MODE_SET
#define TIMER0_WGM_MODE_NORMAL				0x00
#define TIMER0_WGM_MODE_PC_PWM				0x01
#define TIMER0_WGM_MODE_CTC					0x02
#define TIMER0_WGM_MODE_F_PWM				0x03

volatile uint16_t TIMER0_secondPassed;
volatile uint32_t TIMER0_tickCount1ms;

// Initialization
void TIMER0_init ();

// PWM
void TIMER0_setPwm ();
void TIMER0_setDuty (uint8_t stepValue);

// TICK
void TIMER0_enableTick1ms ();
void TIMER0_ISR_OVF_tick1ms();
void TIMER0_ISR_COMP_tick1ms ();

// Set Wave Generation Mode
void TIMER0_setWaveGenerationMode (uint8_t modeNum);

// Set Compare Output Mode
void TIMER0_setCompareOutputMode (uint8_t modeNum);

// Set Clock Mode
void TIMER0_setClockMode (uint8_t modeNum);

// Enable Force Output Compare
void TIMER0_enableForceOutputCompare ();

// Enable Output Compare Interrupt
void TIMER0_enableOutputCompareInterrupt ();

// Disable Output Comapre Interrupt
void TIMER0_disableOutputCompareInterrupt ();

// Enable Overflow Interrupt
void TIMER0_enableOverflowInterrupt ();

// Disable Overflow Interrupt
void TIMER0_disableOverflowInterrupt ();

// Output Compare Register Set
void TIMER0_setOutputCompareReg (uint8_t outputCompareThreshold);

// Timer/Counter Register Set
void TIMER0_setTimerCounterReg (uint8_t count);

// Time Functions
uint32_t TIMER0_millis ();
uint16_t TIMER0_seconds();

#endif /* TIMER0_H_ */