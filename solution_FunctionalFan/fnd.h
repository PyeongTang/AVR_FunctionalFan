/*
 * fnd.h
 *
 * Created: 2024-06-03 오후 8:46:10
 *  Author: Lee JaePyeong
 */ 


#ifndef FND_H_
#define FND_H_

#include "def.h"

#define FND_DDR		DDRA
#define FND_PORT	PORTA

#define SEG_0		0x3F;
#define SEG_1		0x06;
#define SEG_2		0x5B;
#define SEG_3		0x4F;
#define SEG_4		0x66;
#define SEG_5		0x6D;
#define SEG_6		0x7D;
#define SEG_7		0x07;
#define SEG_8		0x7F;
#define SEG_9		0x67;

// DOT G F E D C B A
// Active as 1
#define SEG_SLIDE_SEQ_0		0b00000001
#define SEG_MIDDLE_BAR		0b01000000

typedef struct {
	volatile uint8_t*	Pport;
	uint8_t				status;
	uint8_t				decimalPattern[10];
	uint8_t				slidePattern[6];
} FND;

void FND_init(FND* fnd);
void FND_slide (FND* fnd, uint8_t select);
void FND_middleBar (FND* fnd);
void FND_setValue (FND* fnd, uint8_t value);
void FND_setDecimal (FND* fnd, uint8_t decimalValue);
void FND_allOn(FND* fnd);
void FND_allOff(FND* fnd);
void FND_incValue(FND* fnd);
void FND_decValue(FND* fnd);
void FND_upCount (FND* fnd);
void FND_downCount (FND* fnd);
void FND_zeroShiftPattern (FND* fnd);
void FND_test (FND* fnd);

#endif /* FND_H_ */