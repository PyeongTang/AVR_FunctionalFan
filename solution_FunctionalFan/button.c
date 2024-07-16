/*
 * button.c
 *
 * Created: 2024-06-03 오후 2:51:17
 *  Author: Lee JaePyeong
 */ 

#include "button.h"

void BTN_init (BTN* btn, uint8_t num){
	btn->btnNum = num;
	btn->btnStatus = 0;
	
	// 	Set DDR Input (with clear)
	BTN_DDR &= ~(1 << (btn->btnNum));
	
	// 	Set Internal Pull-up (with set)
	BTN_PORT |= (1 << (btn->btnNum));
}

uint8_t BTN_isPushed (BTN* btn){
// 	Released
	if (BTN_PIN & (1 << btn->btnNum)){
		btn->btnStatus = 0;
		return 0;
	}
// 	Pushed
	else {
		btn->btnStatus = 1;
		return 1;
	}
}

void BTN_enableInterrupt(BTN* btn){
	switch (btn->btnNum) {
		case BTN_0 :
// 			External Interrupt Enable
			EIMSK |= (1 << BTN_0);
// 			External Interrupt Sense Control - Falling Edge
			EICRA |= (1 << ISC01) | (0 << ISC00);
		break;
		
		case BTN_1 :
// 			External Interrupt Enable
			EIMSK |= (1 << BTN_1);
// 			External Interrupt Sense Control - Falling Edge
			EICRA |= (1 << ISC11) | (0 << ISC10);
		break;
		
		case BTN_2 :
// 			External Interrupt Enable
			EIMSK |= (1 << BTN_2);
// 			External Interrupt Sense Control - Falling Edge
			EICRA |= (1 << ISC21) | (0 << ISC20);
		break;
		
		default:break;
	}
	sei();
}