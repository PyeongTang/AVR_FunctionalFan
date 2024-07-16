/*
 * fnd4_c.c
 *
 * Created: 2024-06-04 오후 4:22:18
 *  Author: Lee JaePyeong
 */ 

#include "fnd4.h"

void FND4_init(FND4* fnd4){
	FND4_DATA_DDR	= 0xFF;
	FND4_DATA_PORT	= 0x00;
	
	FND4_SEL_DDR	= 0xFF;
	FND4_SEL_PORT	= 0x00;
	
	fnd4->digitValue[0] = 0;
	fnd4->digitValue[1] = 0;
	fnd4->digitValue[2] = 0;
	fnd4->digitValue[3] = 0;
	
	fnd4->decimalValue = 0;
	
	fnd4->decimalPattern[0] = SEG_0;
	fnd4->decimalPattern[1] = SEG_1;
	fnd4->decimalPattern[2] = SEG_2;
	fnd4->decimalPattern[3] = SEG_3;
	fnd4->decimalPattern[4] = SEG_4;
	fnd4->decimalPattern[5] = SEG_5;
	fnd4->decimalPattern[6] = SEG_6;
	fnd4->decimalPattern[7] = SEG_7;
	fnd4->decimalPattern[8] = SEG_8;
	fnd4->decimalPattern[9] = SEG_9;
}

void FND4_setValuePerDigit(FND4* fnd4, uint8_t digit, uint8_t number){
	if (number > 10){
		fnd4->digitValue[digit] = 9;
	}
	else if (number < 0){
		fnd4->digitValue[digit] = 0;
	}
	else{
		fnd4->digitValue[digit] = number;
	}
	FND4_allOn(fnd4);
}

void FND4_setValue(FND4* fnd4, uint16_t decimalValue){
	fnd4->decimalValue = decimalValue;
	fnd4->digitValue[0] = (decimalValue / 1000)	% 10;
	fnd4->digitValue[1] = (decimalValue / 100)	% 10;
	fnd4->digitValue[2] = (decimalValue / 10)	% 10;
	fnd4->digitValue[3] = (decimalValue / 1)	% 10;
	FND4_allOn(fnd4);
}

void FND4_on(FND4* fnd4, uint8_t digit){
	FND4_SEL_DDR	= 0xFF;
	FND4_SEL_PORT	= 0xFF;
	FND4_SEL_PORT	&= ~(1 << digit);
	FND4_DATA_PORT	= fnd4->decimalPattern[fnd4->digitValue[digit]];
}

void FND4_off(FND4* fnd4, uint8_t digit){
	FND4_SEL_PORT	|= (1 << digit);
}

void FND4_allOff(FND4* fnd4){
	FND4_SEL_PORT	=	0xFF;
}

void FND4_allOn(FND4* fnd4){
	for (uint8_t i = 0; i < 4; i++){
		FND4_on(fnd4, i);
		_delay_ms(1);
	}
}

uint8_t FND4_getOneDigitValue(FND4* fnd4, uint8_t digit){
	return fnd4->digitValue[digit];
}

uint16_t FND4_getDecimalValue(FND4* fnd4){
	return fnd4->decimalValue;
}