/*
 * fnd.c
 *
 * Created: 2024-06-03 오후 8:46:00
 *  Author: Lee JaePyeong
 */ 

#include "fnd.h"

void FND_init(FND* fnd){
	// 	DDR output mode
	FND_DDR		=	0xFF;
	fnd->status =	0x00;
	
	FND_PORT	=	fnd->status;
	
	fnd->decimalPattern[0] = SEG_0;
	fnd->decimalPattern[1] = SEG_1;
	fnd->decimalPattern[2] = SEG_2;
	fnd->decimalPattern[3] = SEG_3;
	fnd->decimalPattern[4] = SEG_4;
	fnd->decimalPattern[5] = SEG_5;
	fnd->decimalPattern[6] = SEG_6;
	fnd->decimalPattern[7] = SEG_7;
	fnd->decimalPattern[8] = SEG_8;
	fnd->decimalPattern[9] = SEG_9;
	
	for (uint8_t i = 0; i < 6; i++){
		fnd->slidePattern[i] = SEG_SLIDE_SEQ_0 << i;
	}
}

void FND_slide (FND* fnd, uint8_t select){
	FND_setValue(fnd, fnd->slidePattern[select]);
}

void FND_middleBar (FND* fnd){
	FND_setValue(fnd, SEG_MIDDLE_BAR);
}

void FND_setValue (FND* fnd, uint8_t value){
	fnd->status = value;
	FND_PORT = fnd->status;
}

void FND_setDecimal (FND* fnd, uint8_t decimalValue){
	fnd->status = fnd->decimalPattern[decimalValue % 10];
	FND_PORT = fnd->status;
}

void FND_allOn(FND* fnd){
	FND_PORT = 0xFF;
}

void FND_allOff(FND* fnd){
	FND_PORT = 0x00;
}

void FND_incValue(FND* fnd){
	fnd->status++;
	fnd->status %= 10;
	FND_setValue(fnd, fnd->status);
}

void FND_decValue(FND* fnd){
	fnd->status--;
	fnd->status %= 10;
	FND_setValue(fnd, fnd->status);
}

void FND_upCount (FND* fnd){
	for (uint8_t i = 0; i < 10; i++){
		FND_setValue(fnd, fnd->decimalPattern[i]);
		_delay_ms(500);
	}
}

void FND_downCount (FND* fnd){
	for (uint8_t i = 0; i < 10; i++){
		FND_setValue(fnd, fnd->decimalPattern[9 - i]);
		_delay_ms(500);
	}
}

void FND_zeroShiftPattern (FND* fnd){
	FND_PORT = 0x00;
	for (uint8_t i = 0; i < 6; i++){
		FND_PORT = (1 << i);
		_delay_ms(200);
	}
}

void FND_test (FND* fnd){
	FND_upCount(fnd);
	FND_downCount(fnd);
	FND_zeroShiftPattern(fnd);
}