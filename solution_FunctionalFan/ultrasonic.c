/*
 * ultrasonic.c
 *
 * Created: 2024-06-24 오후 12:20:01
 *  Author: Lee JaePyeong
 */ 

#include "ultrasonic.h"

// TRIG 10us
// 20Khz	PWM (50us) (DATASHEET)
// 10Khz	PWM (100us)
// 1Khz		PWM (1ms)
// 100hz	PWM (10ms)
//	20hz	PWM (50ms)
// Fast PWM (WGM == 1111),	TOP : OCR1A (Full Step)
//							OCR : OCR1B (High Step) <- Trigger (PB6)
//							ICR : ICP1				<- Echo (PD4)
// Only Output Compare
// High level (10us)
// Ultrasonic using Timer/Counter1 ICP1

// Initialize SR-04 Ultrasonic Sensor
void ULTRA_init (){
// 	Ultrasonic Init
	ULTRASONIC_TRIG_DDR		|=   (1 << ULTRASONIC_TRIG_PORT_NUM);
	ULTRASONIC_TRIG_PORT	&=	~(1 << ULTRASONIC_TRIG_PORT_NUM);
	ULTRASONIC_ECHO_DDR		&=	~(1 << ULTRASONIC_ECHO_PIN_NUM);
	
	ULTRA_distance		= 0;
	ULTRA_startCount	= 0;
	ULTRA_endCount		= 0;
	tempCount			= 0;
	
	TIMER1_init							(TIMER1_CH_B_PORT);
	TIMER1_setWaveGenerationMode		(TIMER1_WGM_MODE_F_PWM_OCR);
	TIMER1_setClockMode					(TIMER1_CLOCK_SELECT_PRE_64);
	TIMER1_setCompareOutputMode			(TIMER1_CH_B, TIMER1_COM_MODE_NON_INVERT);
	TIMER1_setOutputCompareReg			(TIMER1_CH_A, ULTRASONIC_TRIG_FULL_STEP);
	TIMER1_setOutputCompareReg			(TIMER1_CH_B, ULTRASONIC_TRIG_HIGH_STEP);
	TIMER1_setInputCaptureNoiseCanceler	();
	TIMER1_setInputCaptureEdge			(TIMER1_INPUT_CAPTURE_RISING_EDGE);
	TIMER1_enableInputCaptureInterrupt	();
	sei();
}

uint16_t ULTRA_EXE_getDistance (){
// 	Flag check
	if (ULTRA_isDataCaptured){
// 		Clear flag
		ULTRA_isDataCaptured = 0;
	}
	return ULTRA_distance;
}

uint16_t ULTRA_determineDistance(){
	if (ULTRA_startCount < ULTRA_endCount) {
		tempCount = ULTRA_endCount - ULTRA_startCount;
	}
	else{
		tempCount = ULTRA_endCount + (ULTRASONIC_TRIG_FULL_STEP - ULTRA_startCount);
	}
// 	Distance[cm] = countValue[1] * 0.000004[s, Prescaler 4]	* 34000[cm/s] * 0.5 (Round)
	return (uint16_t)((double)(tempCount * 0.000004 * 34000 * 0.5));
}

void ULTRA_ISR_CAPT (){
// 	Posedge Trigger
	if (TCCR1B & (1 << ICES1)){
// 		Capture Data
		ULTRA_startCount = ICR1;
	
// 		Change Polarity
		TIMER1_setInputCaptureEdge(TIMER1_INPUT_CAPTURE_FALLING_EDGE);
	}
// 	Negedge Trigger
	else {
// 		Capture Data
		ULTRA_endCount = ICR1;
		
// 		Change Polarity
		TIMER1_setInputCaptureEdge(TIMER1_INPUT_CAPTURE_RISING_EDGE);
		
// 		Set Flag
		ULTRA_isDataCaptured = 1;
		
// 		Determine Distance
		ULTRA_distance = ULTRA_determineDistance();
	}
}