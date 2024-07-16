/*
 * timer1.h
 *
 * Created: 2024-06-17 오전 9:44:32
 *  Author: Lee JaePyeong
 */ 


#ifndef TIMER1_H_
#define TIMER1_H_

#include "def.h"

#define TIMER1_OC_DDR							DDRB
#define TIMER1_OC_PORT							PORTB

#define TIMER1_CH_A_PORT						PORTB5
#define TIMER1_CH_B_PORT						PORTB6
#define TIMER1_CH_C_PORT						PORTB7

#define TIMER1_CH_A								'A'
#define TIMER1_CH_B								'B'
#define TIMER1_CH_C								'C'

// CLOCK_MODE_SET
#define	TIMER1_CLOCK_SELECT_NO_SOURCE			0x00
#define	TIMER1_CLOCK_SELECT_PRE_1				0x01
#define	TIMER1_CLOCK_SELECT_PRE_8				0x02
#define	TIMER1_CLOCK_SELECT_PRE_64				0x03
#define	TIMER1_CLOCK_SELECT_PRE_256				0x04
#define	TIMER1_CLOCK_SELECT_PRE_1024			0x05
#define	TIMER1_CLOCK_SELECT_EXTERN_FALLING		0x06
#define	TIMER1_CLOCK_SELECT_EXTERN_RISING		0x07

// COM_MODE_SET
#define TIMER1_COM_MODE_NORMAL					0x00
#define TIMER1_COM_MODE_TOGGLE					0x01
#define TIMER1_COM_MODE_NON_INVERT				0x02
#define TIMER1_COM_MODE_INVERT					0x03

// WGM_MODE_SET
#define	TIMER1_WGM_MODE_NORMAL					0x00
#define	TIMER1_WGM_MODE_PC_PWM_8				0x01
#define	TIMER1_WGM_MODE_PC_PWM_9				0x02
#define	TIMER1_WGM_MODE_PC_PWM_10				0x03
#define TIMER1_WGM_MODE_CTC						0x04
#define TIMER1_WGM_MODE_F_PWM_8					0x05
#define TIMER1_WGM_MODE_F_PWM_9					0x06
#define TIMER1_WGM_MODE_F_PWM_10				0x07
#define TIMER1_WGM_MODE_PFC_PWM_ICR				0x08
#define TIMER1_WGM_MODE_PFC_PWM_OCR				0x09
#define TIMER1_WGM_MODE_PC_PWM_ICR				0x0A
#define TIMER1_WGM_MODE_PC_PWM_OCR				0x0B
#define TIMER1_WGM_MODE_CTC_ICR					0x0C
#define TIMER1_WGM_MODE_F_PWM_ICR				0x0E
#define TIMER1_WGM_MODE_F_PWM_OCR				0x0F

// INPUT_CAPTURE_POLARITY
#define TIMER1_INPUT_CAPTURE_RISING_EDGE		0x01
#define TIMER1_INPUT_CAPTURE_FALLING_EDGE		0x00

// 20Khz PWM
#define TIMER1_F_PWM_20K_PRE1_ICR				0x031F // 800 - 1

// 10Khz PWM (LED)
#define	TIMER1_F_PWM_10K_PRE1_ICR				0x063F // 1,600 - 1

// 1 Sec Tick
#define TIMER1_CTC_1_SEC_OCR_TH					0x7A11

uint16_t maxDuty;

// Initialization
void TIMER1_init (uint8_t channel);
void TIMER1_stop ();
uint16_t TIMER1_getCountValue ();

// Set One Sec Tick Modes
void TIMER1_setOneSecTick (uint8_t channel);

// Set PWM
void TIMER1_setPwm20Khz (uint8_t channel);
void TIMER1_setPwm10Khz (uint8_t channel);
void TIMER1_setPwm (uint8_t channel, uint16_t freqency);
void TIMER1_setDuty (uint8_t channel, uint16_t stepValue);
uint16_t TIMER1_getPWMFrequencyValue (uint16_t desiredFrequency);

// Set Wave Generation Mode
void TIMER1_setWaveGenerationMode (uint8_t modeNum);

// Set Compare Output Mode
void TIMER1_setCompareOutputMode (uint8_t channel, uint8_t modeNum);

// Set Clock Mode
void TIMER1_setClockMode (uint8_t modeNum);

// Enable Force Output Compare Interrupt
void TIMER1_enableForceOutputCompare (uint8_t channel);

// Enable / Disable Input Capture Interrupt
void TIMER1_enableInputCaptureInterrupt ();
void TIMER1_disableInputCaptureInterrupt ();

// Enable / Disable Output Compare Interrupt
void TIMER1_enableOutputCompareInterrupt (uint8_t channel);
void TIMER1_disableOutputCompareInterrupt (uint8_t channel);

// Enable / Disable Overflow Interrupt
void TIMER1_enableOverflowInterrupt ();
void TIMER1_disableOverflowInterrupt ();

// Output Compare Register Set
void TIMER1_setOutputCompareReg (uint8_t channel, uint16_t outputCompareThresold);

// Input Capture Register Set
void TIMER1_setInputCaptureReg (uint16_t inputCaptureThreshold);

// Timer/Counter Register Set
void TIMER1_setTimerCounterReg (uint16_t count);

// Input Capture Set
void TIMER1_setInputCaptureNoiseCanceler ();

void TIMER1_setInputCaptureEdge (uint8_t polarity);

#endif /* TIMER1_H_ */