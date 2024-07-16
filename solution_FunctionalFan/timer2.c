/*
 * timer2.c
 *
 * Created: 2024-07-18 오후 12:22:09
 *  Author: Lee JaePyeong
 */ 

/*
//	8비트 타이머의 경우  Timer/Counter 0와 2를 사용한다.
//	PB4 (OC0), PB7 (OC2)

//	1.	TCCR2 (Timer/Counter2 Control Register)
//		- FOC2, WGM20, COM21, COM20, WGM21, CS22, CS21, CS20

	[7] FOC2
	WGM 비트를 non-PWM 모드로 설정했을 경우에만 동작한다.
	그러나 안정성을 위해, PWM 모드 사용 전 클리어를 권장한다.
	해당 비트가 세트되면 비교일치가 곧바로 현재의 파형 출력에 반영된다

	{[6], [3]} WGM21, WGM20
	이 비트들은 카운터의 카운팅 순서를 제어하며, 출력 파형을 결정한다.
	제공하는 기능은 다음과 같다.
	
	WGM21	WGM20	기능					TOP		OCR2갱신	시점		TOV세트시점
	-----------------------------------------------------------------------
	0		0		Normal				0xFF	즉시				MAX
	0		1		Phase Correct PWM	0xFF	TOP				BOTTOM
	1		0		CTC					OCR0	즉시				MAX
	1		1		Fast PWM			0xFF	BOTTOM			MAX
	-----------------------------------------------------------------------

	[5 : 4] COM21, COM20
	이 비트들은 비교 일치 기능의 동작을 제어한다.
	두 비트가 모두 0일 때만 일반 입출력 핀으로 동작한다.
	만약 파형을 출력해야 한다면, 해당 포트의 DDR을 출력 모드로 설정해야 한다.
	
	=================================================
	non-PWM 모드
	COM21	COM20	비고
	-------------------------------------------
	0		0		일반적인 입출력 포트로 동작
	0		1		비교 일치마다 OC2 출력 반전
	1		0		비교 일치에서 OC2 출력 클리어
	1		1		비교 일치에서 OC2 출력 세트
	=================================================

	=================================================
	Fast PWM 모드
	COM21	COM20	비고
	-------------------------------------------
	0		0		일반적인 입출력 포트로 동작
	0		1		-
	1		0		비교 일치에서 OC2 클리어, BOTTOM에서 OC2 세트 (비반전)
	1		1		비교 일치에서 OC2 세트, BOTTOM에서 OC2 클리어 (반전)
	=================================================
	
	=================================================
	Phase Correct PWM 모드
	COM21	COM20	비고
	-------------------------------------------------
	0		0		일반적인 입출력 포트로 동작
	0		1		-
	1		0		업   카운팅 모드에서 비교일치 시 OC2 클리어
					다운 카운팅 모드에서 비교일치 시 OC2 세트
	1		1		업   카운팅 모드에서 비교일치 시 OC2 세트
					다운 카운팅 모드에서 비교일치 시 OC2 클리어
	=================================================
	
	=================================================
	
	[2 : 0] CS22, CS21, CS20
	클록 소스를 선택하는 비트로 기능한다.
	
	=================================================
	CS22	CS21	CS20	비고
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
		f_OC2 = (f_clk) / (2 * N * (1 + OCR2))
		
	* 출력 PWM 파형의 주파수 
		f_OC2PWM = (f_clk) / (N * 256) : (Fast PWM)
		f_OC2PWM = (f_clk) / (N * 510) : (Phase Correct)	


//	2.	TCNT2 (Timer/Counter0 Register)
//		- TCNT2

	[7 : 0]
	사용자는 이 레지스터에 직접 읽거나 쓸 수 있다.
	레지스터에 쓰는 행위는 진행중인 타이머 클록에 대한 비교일치를 막게 된다.
	따라서 카운터 동작중에 레지스터를 수정하는 것은
	TCNT2 레지스터와 OCR2 레지스터 간 비교일치 결과를 잃을 위험이 있다.


//	3.	OCR2 (Output Compare Register)
//		- OCR2

	[7 : 0]
	이 레지스터엔 TCNT2 값과 계속 비교하게 될 8비트의 기준값을 담고있다.
	필요에 따라 비교일치시 인터럽트를 발생시킬 수 있으며,
	단순히 OC2 핀을 통해 파형을 출력할 수 있다.


//	4.	TIMSK (Timer/Counter Interrupt Mask Register)
//		- OCIE2, TOIE2

	[7 : 6] 타이머 2 전용
	[5 : 2] 타이머 1 전용
	
	[1] OCIE2
	이 비트가 세트되어있고, 상태 레지스터의 I-비트가 세트되어 있으면
	Timer/Counter2 비교 일치 인터럽트가 활성화된다.
	비교일치 발생 시 TIFR 레지스터의 OCF2 비트가 세트되고,
	인터럽트가 일어나게 된다.
	
	[0] TOIE2
	이 비트가 세트되어있고, 상태 레지스터의 I-비트가 세트되어 있으면
	Timer/Counter2 오버플로우 인터럽트가 활성화된다.
	오버플로우 발생 시 TIFR 레지스터의 TOV2 비트가 세트되고,
	인터럽트가 일어나게 된다.


//	5.	TIFR (Timer/Counter Interrupt Flag Register)
//		- OCF2, TOV2

	[7 : 6] 타이머 2 전용
	[5 : 2] 타이머 1 전용
	
	[1] OCF2
	Timer/Counter2의 카운터 값 (TCNT2)와 OCR2의 값이 일치할 경우
	세트된다. 인터럽트가 일어나게 되면 관련 인터럽트 핸들링 벡터를 실행하게 되고,
	OCF2는 하드웨어적으로 클리어된다.

*/

#include "timer2.h"

// Initialization
void TIMER2_init (){
// 	Initialize Tick
	TIMER2_tickCount1ms = 0;

// 	Enable TIMER2 Output Compare Port
 	TIMER2_OC_DDR	|=	(1 << TIMER2_OC_PORT_NUM);
 	TIMER2_OC_PORT	&= ~(1 << TIMER2_OC_PORT_NUM);
	 
// 	1 Millisec Timer Enable
// 	TIMER2_enableTick1ms();
	
// 	Global Interrupt Enable
	sei();
}

// Enable 1 ms ISR
void TIMER2_enableTick1ms (){
	
//	Normal (pre 64)
// 	f_OC0 Hz = (16,000,000 Hz / pre)
//	250,000 Hz == 0.000004 s == 0.004 ms
//	1 ms = 0.004 ms * 250
//	Need to count of 250

//	initial point 7 = 256 - 249 (OVF)
//	Output Compare Point 249 (COMP)
	
// 	TIMER2_setWaveGenerationMode		(TIMER2_WGM_MODE_CTC);
	TIMER2_setWaveGenerationMode		(TIMER2_WGM_MODE_NORMAL);
	TIMER2_setCompareOutputMode			(TIMER2_COM_MODE_NORMAL);
	TIMER2_setClockMode					(TIMER2_CLOCK_SELECT_PRE_64);
// 	TIMER2_enableOutputCompareInterrupt	();
	TIMER2_enableOverflowInterrupt		();
// 	TIMER2_setOutputCompareReg			(TIMER2_MILLI_OCR0_VALUE);
}

void TIMER2_setPwm (){
	TIMER2_setWaveGenerationMode		(TIMER2_WGM_MODE_F_PWM);
	TIMER2_setCompareOutputMode			(TIMER2_COM_MODE_NON_INVERT);
	TIMER2_setClockMode					(TIMER2_CLOCK_SELECT_PRE_64);
	// For Segment slide
	// 	TIMER2_enableOutputCompareInterrupt	();
	// 	TIMER2_enableOverflowInterrupt		();
}

void TIMER2_setDuty (uint8_t stepValue){
	TIMER2_setOutputCompareReg(stepValue);
}

// ISR (TIMER2_OVF_vect){
//  	 TIMER2_ISR_OVF_tick1ms();
//   }

//  ISR (TIMER2_COMP_vect){
//  	TIMER2_ISR_COMP_tick1ms();
//  }

void TIMER2_ISR_OVF_tick1ms (){
	if ((++TIMER2_tickCount1ms % 1000) == 0){
		TIMER2_secondPassed++;
	}
	TCNT2 = TIMER2_MILLI_TCNT0_VALUE;
}

void TIMER2_ISR_COMP_tick1ms (){
	TIMER2_tickCount1ms++;
}

// Set Wave Generation Mode
void TIMER2_setWaveGenerationMode (uint8_t modeNum){
	switch (modeNum){
		case TIMER2_WGM_MODE_NORMAL	:
// 			Normal
//			TOP		:	0xFF
			TCCR2	|=	(0 << WGM21) | (0 << WGM20);
		break;
		
		case TIMER2_WGM_MODE_PC_PWM	:
// 			PWM, Phase Correct
//			TOP		:	0xFF
			TCCR2	|=	(0 << WGM21) | (1 << WGM20);
		break;
		
		case TIMER2_WGM_MODE_CTC	:
// 			CTC
//			TOP		:	OCR0
			TCCR2	|=	(1 << WGM21) | (0 << WGM20);
		break;
		
		case TIMER2_WGM_MODE_F_PWM	:
// 			Fast PWM
//			TOP		:	0xFF
			TCCR2	|=	(1 << WGM21) | (1 << WGM20);
		break;
	}
}

// Set Compare Output Mode
void TIMER2_setCompareOutputMode (uint8_t modeNum){
	switch (modeNum){
		case TIMER2_COM_MODE_NORMAL		:
			TCCR2	|=	(0 << COM21) | (0 << COM20);		
		break;
		
		case TIMER2_COM_MODE_TOGGLE		:
			TCCR2	|=	(0 << COM21) | (1 << COM20);
		break;
		
		case TIMER2_COM_MODE_NON_INVERT	:
			TCCR2	|=	(1 << COM21) | (0 << COM20);
		break;
		
		case TIMER2_COM_MODE_INVERT	:
			TCCR2	|=	(1 << COM21) | (1 << COM20);
		break;
	}
}

// Set Clock Mode
void TIMER2_setClockMode (uint8_t modeNum){
	switch (modeNum){
		case TIMER2_CLOCK_SELECT_NO_SOURCE		:
			TCCR2	|=	(0 << CS22) | (0 << CS21) | (0 << CS20);
		break;
		
		case TIMER2_CLOCK_SELECT_PRE_1			:
			TCCR2	|=	(0 << CS22) | (0 << CS21) | (1 << CS20);
		break;
		
		case TIMER2_CLOCK_SELECT_PRE_8			:
			TCCR2	|=	(0 << CS22) | (1 << CS21) | (0 << CS20);
		break;
		
		case TIMER2_CLOCK_SELECT_PRE_32		:
			TCCR2	|=	(0 << CS22) | (1 << CS21) | (1 << CS20);
		break;
		
		case TIMER2_CLOCK_SELECT_PRE_64		:
			TCCR2	|=	(1 << CS22) | (0 << CS21) | (0 << CS20);
		break;
		
		case TIMER2_CLOCK_SELECT_PRE_128		:
			TCCR2	|=	(1 << CS22) | (0 << CS21) | (1 << CS20);
		break;
		
		case TIMER2_CLOCK_SELECT_PRE_256		:
			TCCR2	|=	(1 << CS22) | (1 << CS21) | (0 << CS20);
		break;
		
		case TIMER2_CLOCK_SELECT_PRE_1024		:
			TCCR2	|=	(1 << CS22) | (1 << CS21) | (1 << CS20);
		break;
	}
}

// Enable Force Output Compare
void TIMER2_enableForceOutputCompare (){
	TCCR2	|=	(1 << FOC2);
}

// Enable Output Compare Interrupt
void TIMER2_enableOutputCompareInterrupt (){
	TIMSK	|=	(1 << OCIE2);
}

// Enable Overflow Interrupt
void TIMER2_enableOverflowInterrupt (){
	TIMSK	|=	(1 << TOIE2);
}

// Disable Output Compare Interrupt
void TIMER2_disableOutputCompareInterrupt (){
	TIMSK	&=	~(1 << OCIE2);
	TIMER2_tickCount1ms = 0;
	TIMER2_secondPassed = 0;
}

// Disable Overflow Interrupt
void TIMER2_disableOverflowInterrupt (){
	TIMSK	&=	~(1 << TOIE2);
	TIMER2_tickCount1ms = 0;
	TIMER2_secondPassed = 0;
}

// Output Compare Register Set
void TIMER2_setOutputCompareReg (uint8_t outputCompareThreshold){
	OCR2 = outputCompareThreshold;
}

// Timer/Counter Register Set
void TIMER2_setTimerCounterReg (uint8_t count){
	TCNT2 = count;
}

// Return Milli Seconds
uint32_t TIMER2_millis (){
	return TIMER2_tickCount1ms;
}

uint16_t TIMER2_seconds (){
	return TIMER2_secondPassed;
}