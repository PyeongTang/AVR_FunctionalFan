/*
 * ultraDetector.c
 *
 * Created: 2024-07-17 오후 9:56:59
 *  Author: Lee JaePyeong
 */ 

#include "functionalFan_ultraDetector.h"

// ISR (TIMER3_COMPC_vect){
// 	UD_COMP_ISR();
// }

void UD_COMP_ISR (){
	if ((servoWiperCount++) % 10 == 0){
		if (servoWiperIncrease){
			servoWiperAngle++;
			if (servoWiperAngle >= SERVO_WIPER_ANGLE_MAX){
				servoWiperIncrease = false;
			}
		}
		else {
			servoWiperAngle--;
			if (servoWiperAngle <= SERVO_WIPER_ANGLE_MIN){
				servoWiperIncrease = true;
			}
		}
		SG90_setAngle(PUDWiper, servoWiperAngle);
		servoWiperCount = 0;
	}
}

void UD_init (){
// 	Varaible Initialize
	servoWiperAngle = 0;
	servoWiperCount = 0;
	servoWiperIncrease = true;
	
// 	Wiper Servo Attach
	PUDWiper = &(UDWiper);
	SG90_init(PUDWiper, TIMER_3, TIMER3_CH_B);
	SG90_setAngle(PUDWiper, 90);
	
// 	Pointer Servo Attach
	PUDPointer = &(UDPointer);
	SG90_init(PUDPointer, TIMER_3, TIMER3_CH_A);
	SG90_setAngle(PUDPointer, 90); 
	
// 	Wiper Servo Control Timer Attach
// 	TIMER3_init(TIMER3_CH_C_PORT);
	TIMER3_enableOutputCompareInterrupt(TIMER3_CH_C);
	TIMER3_setCompareOutputMode(TIMER3_CH_C, TIMER3_COM_MODE_NORMAL);
	TIMER3_setWaveGenerationMode(TIMER3_WGM_MODE_NORMAL);
	TIMER3_setOutputCompareReg(TIMER3_CH_C, 3000);
	
// 	Detecting Ultrasonic Attach
	ULTRA_init();
}

void UD_Execute(bool systemOff){
	if (!systemOff){
		TIMER3_enableOutputCompareInterrupt(TIMER3_CH_C);
		if (ULTRA_EXE_getDistance () <= ULTRA_DETECT_TH_CM){
			UD_pointServoHead(PUDPointer, UD_getWiperAngle());
		}
	}
	else{
		TIMER3_disableOutputCompareInterrupt(TIMER3_CH_C);
	}
}

void UD_pointServoHead(SERVO* servo, uint8_t pointingAngle){
	SG90_setAngle(servo, pointingAngle);
}

uint8_t UD_getWiperAngle(){
	return servoWiperAngle;
}