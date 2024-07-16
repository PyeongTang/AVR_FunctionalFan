/*
 * timer3.h
 *
 * Created: 2024-06-25 오후 4:38:17
 *  Author: Lee JaePyeong
 */ 


#ifndef TIMER3_H_
#define TIMER3_H_

#include "def.h"

#define TIMER3_OC_DDR							DDRE
#define TIMER3_OC_PORT							PORTE

#define TIMER3_CH_A_PORT						PORTE3
#define TIMER3_CH_B_PORT						PORTE4
#define TIMER3_CH_C_PORT						PORTE5

#define TIMER3_CH_A								'A'
#define TIMER3_CH_B								'B'
#define TIMER3_CH_C								'C'

// CLOCK_MODE_SET
#define	TIMER3_CLOCK_SELECT_NO_SOURCE			0x00
#define	TIMER3_CLOCK_SELECT_PRE_1				0x01
#define	TIMER3_CLOCK_SELECT_PRE_8				0x02
#define	TIMER3_CLOCK_SELECT_PRE_64				0x03
#define	TIMER3_CLOCK_SELECT_PRE_256				0x04
#define	TIMER3_CLOCK_SELECT_PRE_1024			0x05
#define	TIMER3_CLOCK_SELECT_EXTERN_FALLING		0x06
#define	TIMER3_CLOCK_SELECT_EXTERN_RISING		0x07

// COM_MODE_SET
#define TIMER3_COM_MODE_NORMAL					0x00
#define TIMER3_COM_MODE_TOGGLE					0x01
#define TIMER3_COM_MODE_NON_INVERT				0x02
#define TIMER3_COM_MODE_INVERT					0x03

// WGM_MODE_SET
#define	TIMER3_WGM_MODE_NORMAL					0x00
#define	TIMER3_WGM_MODE_PC_PWM_8				0x01
#define	TIMER3_WGM_MODE_PC_PWM_9				0x02
#define	TIMER3_WGM_MODE_PC_PWM_10				0x03
#define TIMER3_WGM_MODE_CTC						0x04
#define TIMER3_WGM_MODE_F_PWM_8					0x05
#define TIMER3_WGM_MODE_F_PWM_9					0x06
#define TIMER3_WGM_MODE_F_PWM_10				0x07
#define TIMER3_WGM_MODE_PFC_PWM_ICR				0x08
#define TIMER3_WGM_MODE_PFC_PWM_OCR				0x09
#define TIMER3_WGM_MODE_PC_PWM_ICR				0x0A
#define TIMER3_WGM_MODE_PC_PWM_OCR				0x0B
#define TIMER3_WGM_MODE_CTC_ICR					0x0C
#define TIMER3_WGM_MODE_F_PWM_ICR				0x0E
#define TIMER3_WGM_MODE_F_PWM_OCR				0x0F

// 20Khz PWM
#define TIMER3_F_PWM_20K_PRE1_ICR				0x031F // 800 - 1
#define TIMER3_MAX_DUTY							TIMER3_F_PWM_20K_PRE1_ICR

uint16_t maxDuty;

// Initialization
void TIMER3_init (uint8_t channel);
void TIMER3_stop ();
uint16_t TIMER3_getCountValue ();

// Set PWM
void TIMER3_setPwm20Khz (uint8_t channel);
void TIMER3_setPwm (uint8_t channel, uint16_t frequency);
uint16_t TIMER3_getPWMFrequencyValue (uint16_t desiredFrequency);
void TIMER3_setDuty (uint8_t channel, uint16_t stepValue);

// Set Wave Generation Mode
void TIMER3_setWaveGenerationMode (uint8_t modeNum);

// Set Compare Output Mode
void TIMER3_setCompareOutputMode (uint8_t channel, uint8_t modeNum);

// Set Clock Mode
void TIMER3_setClockMode (uint8_t modeNum);

// Enable Force Output Compare Interrupt
void TIMER3_enableForceOutputCompare (uint8_t channel);

// Enable Input Capture Interrupt
void TIMER3_enableInputCaptureInterrupt ();

// Enable / Disable Output Compare Interrupt
void TIMER3_enableOutputCompareInterrupt (uint8_t channel);
void TIMER3_disableOutputCompareInterrupt (uint8_t channel);

// Enable Overflow Interrupt
void TIMER3_enableOverflowInterrupt ();

// Output Compare Register Set
void TIMER3_setOutputCompareReg (uint8_t channel, uint16_t outputCompareThresold);

// Input Capture Register Set
void TIMER3_setInputCaptureReg (uint16_t inputCaptureThreshold);

// Timer/Counter Register Set
void TIMER3_setTimerCounterReg (uint16_t count);

#endif /* TIMER3_H_ */