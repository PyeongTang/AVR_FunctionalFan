/*
 * button.h
 *
 * Created: 2024-06-03 오후 2:51:25
 *  Author: Lee JaePyeong
 */ 


// Usable External Interrupt Pin
// INT[7 : 0] = {PE[7 : 4], PD[3 : 0]}

#ifndef BUTTON_H_
#define BUTTON_H_

#include "def.h"

#define BTN_DDR			DDRD
#define BTN_PORT		PORTD
#define BTN_PIN			PIND

#define BTN_0			0
#define BTN_1			1
#define BTN_2			2

typedef struct {
	uint8_t				btnNum;
	uint8_t				btnStatus;
} BTN;

void BTN_init (BTN* btn, uint8_t num);
void BTN_enableInterrupt(BTN* btn);
uint8_t BTN_isPushed (BTN* btn);

#endif /* BUTTON_H_ */