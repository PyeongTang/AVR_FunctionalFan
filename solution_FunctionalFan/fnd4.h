/*
 * fnd4.h
 *
 * Created: 2024-06-04 오후 4:22:35
 *  Author: Lee JaePyeong
 */ 


#ifndef FND4_H_
#define FND4_H_

#include "def.h"
#include "fnd.h"

#define FND4_DATA_DDR	DDRC
#define FND4_DATA_PORT	PORTC

#define FND4_SEL_DDR	DDRG
#define FND4_SEL_PORT	PORTG

typedef struct {
	uint8_t  digitValue[4];
	uint16_t decimalValue;
	uint8_t  decimalPattern[10];
} FND4;

void FND4_init(FND4* fnd4);
void FND4_setValuePerDigit(FND4* fnd4, uint8_t digit, uint8_t number);
void FND4_setValue(FND4* fnd4, uint16_t decimalValue);
void FND4_on(FND4* fnd4, uint8_t digit);
void FND4_off(FND4* fnd4, uint8_t digit);
void FND4_allOn(FND4* fnd4);
void FND4_allOff(FND4* fnd4);
uint8_t FND4_getOneDigitValue(FND4* fnd4, uint8_t digit);
uint16_t FND4_getDecimalValue(FND4* fnd4);

#endif /* FND4_H_ */