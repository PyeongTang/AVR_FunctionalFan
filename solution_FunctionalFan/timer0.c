/*
 * timer0.c
 *
 * Created: 2024-06-17 오전 8:48:00
 *  Author: Lee JaePyeong
 */ 

/*
//	8비트 타이머의 경우  Timer/Counter 0와 2를 사용한다.
//	PB4 (OC0), PB7 (OC2)

//	1.	TCCR0 (Timer/Counter0 Control Register)
//		- FOC0, WGM00, COM01, COM00, WGM01, CS02, CS01, CS00

	[7] FOC0
	WGM 비트를 non-PWM 모드로 설정했을 경우에만 동작한다.
	그러나 안정성을 위해, PWM 모드 사용 전 클리어를 권장한다.
	해당 비트가 세트되면 비교일치가 곧바로 현재의 파형 출력에 반영된다

	{[6], [3]} WGM01, WGM00
	이 비트들은 카운터의 카운팅 순서를 제어하며, 출력 파형을 결정한다.
	제공하는 기능은 다음과 같다.
	
	WGM01	WGM00	기능					TOP		OCR0갱신	시점		TOV세트시점
	-----------------------------------------------------------------------
	0		0		Normal				0xFF	즉시				MAX
	0		1		Phase Correct PWM	0xFF	TOP				BOTTOM
	1		0		CTC					OCR0	즉시				MAX
	1		1		Fast PWM			0xFF	BOTTOM			MAX
	-----------------------------------------------------------------------

	[5 : 4] COM01, COM00
	이 비트들은 비교 일치 기능의 동작을 제어한다.
	두 비트가 모두 0일 때만 일반 입출력 핀으로 동작한다.
	만약 파형을 출력해야 한다면, 해당 포트의 DDR을 출력 모드로 설정해야 한다.
	
	=================================================
	non-PWM 모드
	COM01	COM00	비고
	-------------------------------------------
	0		0		일반적인 입출력 포트로 동작
	0		1		비교 일치마다 OC0 출력 반전
	1		0		비교 일치에서 OC0 출력 클리어
	1		1		비교 일치에서 OC0 출력 세트
	=================================================

	=================================================
	Fast PWM 모드
	COM01	COM00	비고
	-------------------------------------------
	0		0		일반적인 입출력 포트로 동작
	0		1		-
	1		0		비교 일치에서 OC0 클리어, BOTTOM에서 OC0 세트 (비반전)
	1		1		비교 일치에서 OC0 세트, BOTTOM에서 OC0 클리어 (반전)
	=================================================
	
	=================================================
	Phase Correct PWM 모드
	COM01	COM00	비고
	-------------------------------------------------
	0		0		일반적인 입출력 포트로 동작
	0		1		-
	1		0		업   카운팅 모드에서 비교일치 시 OC0 클리어
					다운 카운팅 모드에서 비교일치 시 OC0 세트
	1		1		업   카운팅 모드에서 비교일치 시 OC0 세트
					다운 카운팅 모드에서 비교일치 시 OC0 클리어
	=================================================
	
	=================================================
	
	[2 : 0] CS02, CS01, CS00
	클록 소스를 선택하는 비트로 기능한다.
	
	=================================================
	CS02	CS01	CS00	비고
	-------------------------------------------------
	0		0		0		클록 소스 없음 (Timer/Counter 중지)
	0		0		1		분주기 값 1
	0		1		0		분주기 값 8
	0		1		1		분주기 값 32
	1		0		0		분주기 값 64
	1		0		1		분주기 값 128
	1		1		0		분주기 값 256
	1		1		1		분주기 값 1024
	=================================================
	* 출력 파형의 주파수
		f_OC0 = (f_clk) / (2 * N * (1 + OCR0))
		
	* 출력 PWM 파형의 주파수 
		f_OC0PWM = (f_clk) / (N * 256) : (Fast PWM)
		f_OC0PWM = (f_clk) / (N * 510) : (Phase Correct)	


//	2.	TCNT0 (Timer/Counter0 Register)
//		- TCNT0

	[7 : 0]
	사용자는 이 레지스터에 직접 읽거나 쓸 수 있다.
	레지스터에 쓰는 행위는 진행중인 타이머 클록에 대한 비교일치를 막게 된다.
	따라서 카운터 동작중에 레지스터를 수정하는 것은
	TCNT0 레지스터와 OCR0 레지스터 간 비교일치 결과를 잃을 위험이 있다.


//	3.	OCR0 (Output Compare Register)
//		- OCR0

	[7 : 0]
	이 레지스터엔 TCNT0 값과 계속 비교하게 될 8비트의 기준값을 담고있다.
	필요에 따라 비교일치시 인터럽트를 발생시킬 수 있으며,
	단순히 OC0 핀을 통해 파형을 출력할 수 있다.


//	4.	TIMSK (Timer/Counter Interrupt Mask Register)
//		- OCIE0, TOIE0

	[7 : 6] 타이머 2 전용
	[5 : 2] 타이머 1 전용
	
	[1] OCIE0
	이 비트가 세트되어있고, 상태 레지스터의 I-비트가 세트되어 있으면
	Timer/Counter0 비교 일치 인터럽트가 활성화된다.
	비교일치 발생 시 TIFR 레지스터의 OCF0 비트가 세트되고,
	인터럽트가 일어나게 된다.
	
	[0] TOIE0
	이 비트가 세트되어있고, 상태 레지스터의 I-비트가 세트되어 있으면
	Timer/Counter0 오버플로우 인터럽트가 활성화된다.
	오버플로우 발생 시 TIFR 레지스터의 TOV0 비트가 세트되고,
	인터럽트가 일어나게 된다.


//	5.	TIFR (Timer/Counter Interrupt Flag Register)
//		- OCF0, TOV0

	[7 : 6] 타이머 2 전용
	[5 : 2] 타이머 1 전용
	
	[1] OCF0
	Timer/Counter0의 카운터 값 (TCNT0)와 OCR0의 값이 일치할 경우
	세트된다. 인터럽트가 일어나게 되면 관련 인터럽트 핸들링 벡터를 실행하게 되고,
	OCF0는 하드웨어적으로 클리어된다.

*/

#include "timer0.h"

// Initialization
void TIMER0_init (){
// 	Initialize Tick
	TIMER0_tickCount1ms = 0;

// 	Enable TIMER0 Output Compare Port
 	TIMER0_OC_DDR	|=	(1 << TIMER0_OC_PORT_NUM);
 	TIMER0_OC_PORT	&= ~(1 << TIMER0_OC_PORT_NUM);
	 
// 	1 Millisec Timer Enable
	TIMER0_enableTick1ms();
	
// 	Global Interrupt Enable
	sei();
}

// Enable 1 ms ISR
void TIMER0_enableTick1ms (){
	
//	Normal (pre 64)
// 	f_OC0 Hz = (16,000,000 Hz / pre)
//	250,000 Hz == 0.000004 s == 0.004 ms
//	1 ms = 0.004 ms * 250
//	Need to count of 250

//	initial point 7 = 256 - 249 (OVF)
//	Output Compare Point 249 (COMP)
	
// 	TIMER0_setWaveGenerationMode		(TIMER0_WGM_MODE_CTC);
	TIMER0_setWaveGenerationMode		(TIMER0_WGM_MODE_NORMAL);
	TIMER0_setCompareOutputMode			(TIMER0_COM_MODE_NORMAL);
	TIMER0_setClockMode					(TIMER0_CLOCK_SELECT_PRE_64);
// 	TIMER0_enableOutputCompareInterrupt	();
	TIMER0_enableOverflowInterrupt		();
// 	TIMER0_setOutputCompareReg			(TIMER0_MILLI_OCR0_VALUE);
}

void TIMER0_setPwm (){
	TIMER0_setWaveGenerationMode		(TIMER0_WGM_MODE_F_PWM);
	TIMER0_setCompareOutputMode			(TIMER0_COM_MODE_NON_INVERT);
	TIMER0_setClockMode					(TIMER0_CLOCK_SELECT_PRE_64);
	// For Segment slide
	// 	TIMER2_enableOutputCompareInterrupt	();
	// 	TIMER2_enableOverflowInterrupt		();
}

void TIMER0_setDuty (uint8_t stepValue){
	TIMER0_setOutputCompareReg(stepValue);
}

// ISR (TIMER0_OVF_vect){
//  	 TIMER0_ISR_OVF_tick1ms();
//   }

//  ISR (TIMER0_COMP_vect){
//  	TIMER0_ISR_COMP_tick1ms();
//  }

void TIMER0_ISR_OVF_tick1ms (){
	if ((++TIMER0_tickCount1ms % 1000) == 0){
		TIMER0_secondPassed++;
	}
	TCNT0 = TIMER0_MILLI_TCNT0_VALUE;
}

void TIMER0_ISR_COMP_tick1ms (){
	TIMER0_tickCount1ms++;
}

// Set Wave Generation Mode
void TIMER0_setWaveGenerationMode (uint8_t modeNum){
	switch (modeNum){
		case TIMER0_WGM_MODE_NORMAL	:
// 			Normal
//			TOP		:	0xFF
			TCCR0	|=	(0 << WGM01) | (0 << WGM00);
		break;
		
		case TIMER0_WGM_MODE_PC_PWM	:
// 			PWM, Phase Correct
//			TOP		:	0xFF
			TCCR0	|=	(0 << WGM01) | (1 << WGM00);
		break;
		
		case TIMER0_WGM_MODE_CTC	:
// 			CTC
//			TOP		:	OCR0
			TCCR0	|=	(1 << WGM01) | (0 << WGM00);
		break;
		
		case TIMER0_WGM_MODE_F_PWM	:
// 			Fast PWM
//			TOP		:	0xFF
			TCCR0	|=	(1 << WGM01) | (1 << WGM00);
		break;
	}
}

// Set Compare Output Mode
void TIMER0_setCompareOutputMode (uint8_t modeNum){
	switch (modeNum){
		case TIMER0_COM_MODE_NORMAL		:
			TCCR0	|=	(0 << COM01) | (0 << COM00);		
		break;
		
		case TIMER0_COM_MODE_TOGGLE		:
			TCCR0	|=	(0 << COM01) | (1 << COM00);
		break;
		
		case TIMER0_COM_MODE_NON_INVERT	:
			TCCR0	|=	(1 << COM01) | (0 << COM00);
		break;
		
		case TIMER0_COM_MODE_INVERT	:
			TCCR0	|=	(1 << COM01) | (1 << COM00);
		break;
	}
}

// Set Clock Mode
void TIMER0_setClockMode (uint8_t modeNum){
	switch (modeNum){
		case TIMER0_CLOCK_SELECT_NO_SOURCE		:
			TCCR0	|=	(0 << CS02) | (0 << CS01) | (0 << CS00);
		break;
		
		case TIMER0_CLOCK_SELECT_PRE_1			:
			TCCR0	|=	(0 << CS02) | (0 << CS01) | (1 << CS00);
		break;
		
		case TIMER0_CLOCK_SELECT_PRE_8			:
			TCCR0	|=	(0 << CS02) | (1 << CS01) | (0 << CS00);
		break;
		
		case TIMER0_CLOCK_SELECT_PRE_32		:
			TCCR0	|=	(0 << CS02) | (1 << CS01) | (1 << CS00);
		break;
		
		case TIMER0_CLOCK_SELECT_PRE_64		:
			TCCR0	|=	(1 << CS02) | (0 << CS01) | (0 << CS00);
		break;
		
		case TIMER0_CLOCK_SELECT_PRE_128		:
			TCCR0	|=	(1 << CS02) | (0 << CS01) | (1 << CS00);
		break;
		
		case TIMER0_CLOCK_SELECT_PRE_256		:
			TCCR0	|=	(1 << CS02) | (1 << CS01) | (0 << CS00);
		break;
		
		case TIMER0_CLOCK_SELECT_PRE_1024		:
			TCCR0	|=	(1 << CS02) | (1 << CS01) | (1 << CS00);
		break;
	}
}

// Enable Force Output Compare
void TIMER0_enableForceOutputCompare (){
	TCCR0	|=	(1 << FOC0);
}

// Enable Output Compare Interrupt
void TIMER0_enableOutputCompareInterrupt (){
	TIMSK	|=	(1 << OCIE0);
}

// Enable Overflow Interrupt
void TIMER0_enableOverflowInterrupt (){
	TIMSK	|=	(1 << TOIE0);
}

// Disable Output Compare Interrupt
void TIMER0_disableOutputCompareInterrupt (){
	TIMSK	&=	~(1 << OCIE0);
	TIMER0_tickCount1ms = 0;
	TIMER0_secondPassed = 0;
}

// Disable Overflow Interrupt
void TIMER0_disableOverflowInterrupt (){
	TIMSK	&=	~(1 << TOIE0);
	TIMER0_tickCount1ms = 0;
	TIMER0_secondPassed = 0;
}

// Output Compare Register Set
void TIMER0_setOutputCompareReg (uint8_t outputCompareThreshold){
	OCR0 = outputCompareThreshold;
}

// Timer/Counter Register Set
void TIMER0_setTimerCounterReg (uint8_t count){
	TCNT0 = count;
}

// Return Milli Seconds
uint32_t TIMER0_millis (){
	return TIMER0_tickCount1ms;
}

uint16_t TIMER0_seconds (){
	return TIMER0_secondPassed;
}