/*
 * timer1.c
 *
 * Created: 2024-06-17 오전 9:44:23
 *  Author: Lee JaePyeong
 */ 

/*
//	PB5 (OC1A), PB6 (OC1B), PB7 (OC1C)

//	1.	TCCR1A (Timer/Counter1 Control Register A)
//		- COM1A1, COM1A0, COM1B1, COM1B0, COM1C1, COM1C0, WGM11, WGM1
	[7 : 6] COM1A1, COM1A0
	[5 : 4] COM1B1, COM1B0
	[3 : 2] COM1C1, COM1C0
	
	이 비트들은 비교 일치 기능의 동작을 제어한다.
	두 비트가 모두 0일 때만 일반 입출력 핀으로 동작한다.
	만약 파형을 출력해야 한다면, 해당 포트의 DDR을 출력 모드로 설정해야 한다.
	* OC1A (PB4), OC1B (PB6), OC1C (PB7)
	* OC3A (PE3), OC3B (PE4), OC3C (PE5)
	
	만약 OC1_가 일반 입출력 핀에 연결되어있을 경우, (_ : A, B, C)
	COM 비트의 기능은 WGM 비트값에 따라 달라진다.
	
	=================================================
	non-PWM 모드
	COM1_1	COM1_0	비고
	-------------------------------------------
	0		0		일반적인 입출력 포트로 동작
	0		1		비교 일치마다 OC1_ 출력 반전
	1		0		비교 일치에서 OC1_ 출력 클리어
	1		1		비교 일치에서 OC1_ 출력 세트
	=================================================

	=================================================
	Fast PWM 모드
	COM1_1	COM1_0	비고
	-------------------------------------------
	0		0		일반적인 입출력 포트로 동작
	
	0		1		WGM[3 : 0] = 15일 때, 비교일치마다 OC1A 토글,
					OC1B와 OC1C는 연결되지 않음. (입출력 핀으로 동작)
					15가 아닌 이외의 WGM값에 대해 OC1_은 연결되지 않음.
					
	1		0		비교 일치에서 OC1_ 클리어,	BOTTOM에서 OC1_ 세트 (비반전)
	1		1		비교 일치에서 OC1_ 세트,		BOTTOM에서 OC1_ 클리어 (반전)
	=================================================
	
	=================================================
	Phase Correct & Phase and Frequency Correct PWM 모드
	COM1_1	COM1_0	비고
	-------------------------------------------------
	0		0		일반적인 입출력 포트로 동작
	
	0		1		WGM[3 : 0] = 9 or 11일 때, 비교 일치마다 OC1A 토글,
					OC1B와 OC1C는 연결되지 않음. (입출력 핀으로 동작)
					9 or 11이 아닌 이외의 WGM값에 대해 OC1_은 연결되지 않음.
					
	1		0		업   카운팅 모드에서 비교일치 시 OC1_ 클리어
					다운 카운팅 모드에서 비교일치 시 OC1_ 세트
	1		1		업   카운팅 모드에서 비교일치 시 OC1_ 세트
					다운 카운팅 모드에서 비교일치 시 OC1_ 클리어
	=================================================
	
	[1 : 0]	WGM11, WGM10
	* TCCR1B 레지스터의 [4 : 3] 위치에 있는 WGM13, WGM12와 함께 사용한다.
	WGM13	WGM12	WGM11	WGM10	동작									TOP			OCR1_ 갱신		TOV1 플래그 세트
	----------------------------------------------------------------------------------------------------------------
	0		0		0		0		Normal								0xFFFF		즉시				MAX
	0		0		0		1		PWM, Phase Correct, 8-bit			0x00FF		TOP				BOTTOM
	0		0		1		0		PWM, Phase Correct, 9-bit			0x01FF		TOP				BOTTOM
	0		0		1		1		PWM, Phase Correct, 10-bit			0x03FF		TOP				BOTTOM
	0		1		0		0		CTC									OCR1A		즉시				MAX
	0		1		0		1		Fast PWM, 8-bit						0x00FF		BOTTOM			TOP
	0		1		1		0		Fast PWM, 9-bit						0x01FF		BOTTOM			TOP
	0		1		1		1		Fast PWM, 10-bit					0x03FF		BOTTOM			TOP
	1		0		0		0		PWM, Phase and Frequency Correct	ICR1		BOTTOM			BOTTOM
	1		0		0		1		PWM, Phase and Frequency Correct	OCR1A		BOTTOM			BOTTOM
	1		0		1		0		PWM, Phase Correct					ICR1		TOP				BOTTOM
	1		0		1		1		PWM, Phase Correct					OCR1A		TOP				BOTTOM
	1		1		0		0		CTC									ICR1		즉시				MAX
	1		1		0		1		-									-			-				-
	1		1		1		0		Fast PWM							ICR1		BOTTOM			TOP
	1		1		1		1		Fast PWM							OCR1A		BOTTOM			TOP


//	2.	TCCR1B (Timer/Counter1 Control Register B)
//		- ICNC1, ICES1, WGM13, WGM12, CS12, CS11, CS10

	[7] ICNC1
	이 비트를 세트하면 Input Capture Noise Canceler를 활성화 한다.
	Noise Canceler가 활성화되면, Input Capture Pin (ICPn) 값이 필터링된다.
	필터링 기능은 출력을 위해 4개의 연속적이고 동일한 값을 가진 샘플을 필요로 하기에,
	Input Capture를 위해 4개의 오실레이터 사이클만큼 딜레이 된다.
	
	[6] ICES1
	이 비트는 ICPn 핀을 통해 입력되는 데이터 중 캡처 이벤트를 발생시킬 에지를 결정한다.
	클리어되면 Negedge가 트리거로 동작하고,
	세트되면 Posedge가 트리거로 동작하여 Input Event를 캡처한다.
	
	[5] -
	
	[4 : 3] WGM13, WGM12
	TCCR1A를 참고한다.
	
	[2 : 0] CS12, CS11, CS10
	클록 소스를 선택하는 비트로 기능한다.
	
	=================================================
	CS12	CS11	CS10	비고
	-------------------------------------------------
	0		0		0		클록 소스 없음 (Timer/Counter 중지)
	0		0		1		분주기 값 1
	0		1		0		분주기 값 8
	0		1		1		분주기 값 64
	1		0		0		분주기 값 256
	1		0		1		분주기 값 1024
	1		1		0		외부 클록 소스 사용, Negedge 감지
	1		1		1		외부 클록 소스 사용, Posedge 감지
	=================================================
	* 출력 파형의 주파수
	f_OC1A = (f_clk) / (2 * N * (1 + OCR1A))
	
	* 출력 PWM 파형의 주파수
	f_OC1_PWM = (f_clk) / (N * (1 + TOP))	: (Fast PWM)
	f_OC1_PWM = (f_clk) / (2 * N * TOP)		: (Phase Correct, Phase and Frequency Correct)
		


//	3.	TCCR1C (Timer/Counter1 Control Register C)
//		- FOC1A, FOC1B, FOC1C

	[7 : 5] FOC1A, FOC1B, FOC1C
	WGM 비트를 non-PWM 모드로 설정했을 경우에만 동작한다.
	그러나 안정성을 위해, PWM 모드 사용 전 클리어를 권장한다.
	해당 비트가 세트되면 비교일치가 곧바로 현재의 파형 출력에 반영된다
	OC1A, OC1B, OC1C 출력 파형은 COM 비트에 따라 다르게 선택된다.
	
	[4 : 0] -


//	4.	TCNT1 (Timer/Counter1)
//		- TCNT1

	[15 : 0]
	TCNT1 = {TCNT1[15 : 8], TCNT1[7 : 0]} = {TCNT1H, TCNT1L}
	
	사용자는 이 레지스터에 직접 읽거나 쓸 수 있다.
	레지스터에 쓰는 행위는 진행중인 타이머 클록에 대한 비교일치를 막게 된다.
	따라서 카운터 동작중에 레지스터를 수정하는 것은
	TCNT1 레지스터와 OCR1 레지스터 간 비교일치 결과를 잃을 위험이 있다.


//	5.	OCR1A, B, C	(Output Compare Register 1 A, B, C)
//		- OCR1A, OCR1B, OCR1C

	[15 : 0]
	이 레지스터엔 TCNT1 값과 계속 비교하게 될 16비트의 기준값을 담고있다.
	필요에 따라 비교일치시 인터럽트를 발생시킬 수 있으며,
	단순히 OC1_ 핀을 통해 파형을 출력할 수 있다.
	
	16비트 레지스터에 값을 써 넣기 위해,
	반드시 High Byte를 먼저 써 넣은 후 Low Byte를 써 넣어야 한다.
	
	반대로 16비트 레지스터의 값을 읽기 위해,
	반드시 Low Byte를 먼저 읽은 후 High Byte를 읽어야 한다.
	
	16비트 레지스터인 TCNT1, OCR1A, OCR1B, OCR1C, ICR1에 값을 읽거나 쓰기 위해선
	SREG 내 인터럽트를 비활성화 해야 한다.
		unsigned char sreg = SREG;
		disable_interrupt();
		unsigned int i = TCNT1; //	Read Access
		TCNT1 = j;				//	Write Access
		SREG = sreg;
		


//	6.	ICR1 (Input Capture Register 1)
// 		- ICR1

	[15 : 0]
	
	인풋 캡처란 ICP1 핀에 이벤트가 발생할 때 마다
	사건의 발생 시각을 감지하여 카운터의 TOP값을 변경한다.
	(TCNT1의 값을 ICR1 레지스터에 쓰는 것을 의미한다.)


//	7.	TIMSK (Timer/Counter Interrupt Mask Register)
//		- TICIE1, OCIE1A, OCIE1B, TOIE1

	[7 : 6] 타이머 2 전용
	
	[5] TICIE1
	이 비트가 세트되고 상태 레지스터의 I-flag가 세트되어있다면,
	Timer/Counter1의 인풋 캡처 인터럽트가 활성화된다.
	관련 인터럽트 벡터는 TIFR 내 ICF1 flag가 활성화 될 때 실행된다.
	
	[4] OCIE1A
	이 비트가 세트되고 상태 레지스터의 I-flag가 세트되어있다면,
	Timer/Counter1 A의 비교일치 인터럽트가 활성화된다.
	관련 인터럽트 벡터는 TIFR 내 OCF1A flag가 활성화 될 때 실행된다.
	
	[3] OCIE1B
	이 비트가 세트되고 상태 레지스터의 I-flag가 세트되어있다면,
	Timer/Counter1 B의 비교일치 인터럽트가 활성화된다.
	관련 인터럽트 벡터는 TIFR 내 OCF1B flag가 활성화 될 때 실행된다.
	
	[2] TOIE1
	이 비트가 세트되고 상태 레지스터의 I-flag가 세트되어있다면,
	Timer/Counter1의 오버플로우 인터럽트가 활성화된다.
	관련 인터럽트 벡터는 TIFR 내 TOV1 flag가 활성화 될 때 실행된다.
	
	[1 : 0] 타이머 0 전용


//	8.	ETIMSK (Extended Timer/Counter Interrupt Mask Register)
//		- TICIE3, OCIE3A, OCIE3B, TOIE3, OCIE3C, OCIE1C

	[7 : 6] -
	
	[5] TICIE3
	이 비트가 세트되고 상태 레지스터의 I-flag가 세트되어있다면,
	Timer/Counter3의 인풋 캡처 인터럽트가 활성화된다.
	관련 인터럽트 벡터는 ETIFR 내 ICF3 flag가 활성화 될 때 실행된다.
	
	[4] OCIE3A
	이 비트가 세트되고 상태 레지스터의 I-flag가 세트되어있다면,
	Timer/Counter3 A의 비교일치 인터럽트가 활성화된다.
	관련 인터럽트 벡터는 ETIFR 내 OCF3A flag가 활성화 될 때 실행된다.
	
	[3] OCIE3B
	이 비트가 세트되고 상태 레지스터의 I-flag가 세트되어있다면,
	Timer/Counter3 B의 비교일치 인터럽트가 활성화된다.
	관련 인터럽트 벡터는 ETIFR 내 OCF3B flag가 활성화 될 때 실행된다.
	
	[2] TOIE3
	이 비트가 세트되고 상태 레지스터의 I-flag가 세트되어있다면,
	Timer/Counter3의 오버플로우 인터럽트가 활성화된다.
	관련 인터럽트 벡터는 ETIFR 내 TOV3 flag가 활성화 될 때 실행된다.
	
	[1] OCIE3C
	이 비트가 세트되고 상태 레지스터의 I-flag가 세트되어있다면,
	Timer/Counter3 C의 비교일치 인터럽트가 활성화된다.
	관련 인터럽트 벡터는 ETIFR 내 OCF3C flag가 활성화 될 때 실행된다.
	
	[0] OCIE1C
	이 비트가 세트되고 상태 레지스터의 I-flag가 세트되어있다면,
	Timer/Counter1 C의 비교일치 인터럽트가 활성화된다.
	관련 인터럽트 벡터는 ETIFR 내 OCF1C flag가 활성화 될 때 실행된다.


//	9.	TIFR (Timer/Counter Interrupt Flag Register)
//		- ICF1, OCF1A, OCF1B, TOV1

	[7 : 6] 타이머 2 전용
	
	[5] ICF1
	ICP1 핀에 캡처 이벤트가 발생하면 세트된다.
	ICR1 레지스터가 WGM에 의해 TOP 값으로 사용될 경우,
	카운터 값이 TOP 값에 도달했을 때 ICF1 플래그가 세트된다.
	인풋 캡처 인터럽트 벡터가 실행되었을 때 ICF1 플래그는 자동적으로 클리어된다.
	또는, ICF1 비트에 1을 써 넣으므로써 클리어 시킬 수 있다.
	
	[4] OCF1A
	TCNT1 레지스터 내 카운트 값이 OCR1A와 같아지는 비교일치 시점에
	OCF1A 플래그가 세트된다.
	OCF1A 플래그는 비교일치 A 인터럽트 벡터가 실행된 후 자동으로 클리어된다.
	또는, OCF1A 비트에 1을 써 넣으므로써 클리어 시킬 수 있다.
	
	[3] OCF1B
	TCNT1 레지스터 내 카운트 값이 OCR1B와 같아지는 비교일치 시점에
	OCF1B 플래그가 세트된다.
	OCF1B 플래그는 비교일치 B 인터럽트 벡터가 실행된 후 자동으로 클리어된다.
	또는, OCF1B 비트에 1을 써 넣으므로써 클리어 시킬 수 있다.
	
	[2] TOV1
	TOV1 플래그의 동작은 WGM 비트값에 의존한다.
	즉, Normal Mode와 CTC 모드에선 TOV1 플래그는 오버플로우 플래그로 동작한다.
	TOV1 플래그는 Timer/Counter1 오버플로우 인터럽트 벡터가 실행되면 자동으로 클리어된다.
	또는, TOV1 비트에 1을 써 넣으므로써 클리어 시킬 수 있다.

//	10.	ETIFR (Extended Timer/Counter Interrupt Flag Register)
//		- ICF3, OCF3A, OCF3B, TOV3, OCF3C, OCF1C

	[7 : 6] -
	
	[5] ICF3
	이 플래그는 ICP3 핀에 캡처 이벤트가 발생할 때 세트된다.
	ICR3 레지스터를 TOP 값으로 사용하도록 WGM 비트가 설정되었다면,
	카운터 값이 TOP 값과 일치할 때 ICF3 플래그가 세트된다.
	
	[4] OCF3A
	이 플래그는 TCNT3 레지스터 내 카운트 값이
	OCR3A 내 값과 비교일치가 일어난 직후 세트된다.
	단, FOC3A를 통해 비교일치를 실행한 경우 OCF3A 플래그는 세트되지 않는다.
	OCF3A 비트는 비교일치 3 A 인터럽트 벡터가 실행된 이후 자동적으로 클리어된다.
	또는, OCF3A 비트에 직접 1을 써 넣어 클리어 시킬 수 있다.
	
	[3] OCF3B
	이 플래그는 TCNT3 레지스터 내 카운트 값이
	OCR3B 내 값과 비교일치가 일어난 직후 세트된다.
	단, FOC3B를 통해 비교일치를 실행한 경우 OCF3B 플래그는 세트되지 않는다.
	OCF3B 비트는 비교일치 3 B 인터럽트 벡터가 실행된 이후 자동적으로 클리어된다.
	또는, OCF3B 비트에 직접 1을 써 넣어 클리어 시킬 수 있다.
	
	[2] TOV3
	TOV3 플래그의 동작은 WGM 비트값에 의존한다.
	즉, Normal Mode와 CTC 모드에선 TOV3 플래그는 오버플로우 플래그로 동작한다.
	TOV3 플래그는 Timer/Counter3 오버플로우 인터럽트 벡터가 실행되면 자동으로 클리어된다.
	또는, TOV3 비트에 1을 써 넣으므로써 클리어 시킬 수 있다.
	
	[1] OCF3C
	이 플래그는 TCNT3 레지스터 내 카운트 값이
	OCR3C 내 값과 비교일치가 일어난 직후 세트된다.
	단, FOC3C를 통해 비교일치를 실행한 경우 OCF3C 플래그는 세트되지 않는다.
	OCF3C 비트는 비교일치 3 C 인터럽트 벡터가 실행된 이후 자동적으로 클리어된다.
	또는, OCF3C 비트에 직접 1을 써 넣어 클리어 시킬 수 있다.
	
	[0] OCF1C
	이 플래그는 TCNT1 레지스터 내 카운트 값이
	OCR1C 내 값과 비교일치가 일어난 직후 세트된다.
	단, FOC1C를 통해 비교일치를 실행한 경우 OCF1C 플래그는 세트되지 않는다.
	OCF1C 비트는 비교일치 1 C 인터럽트 벡터가 실행된 이후 자동적으로 클리어된다.
	또는, OCF1C 비트에 직접 1을 써 넣어 클리어 시킬 수 있다.
*/

#include "timer1.h"

/*
ISR (TIMER1_OVF_vect){
// 	Interrupt Vector Executed
//	when Overflow occured (TCNT1 == MAX)
}

ISR (TIMER1_COMPA_vect){
// 	Interrupt Vector Executed
//	when Compare matched (TCNT1 == OCR1A)
}

ISR (TIMER1_COMPB_vect){
// 	Interrupt Vector Executed
//	when Compare matched (TCNT1 == OCR1B)
}

ISR (TIMER1_COMPC_vect){
// 	Interrupt Vector Executed
//	when Compare matched (TCNT1 == OCR1C)
}

ISR (TIMER1_CAPT_vect){
// 	Interrupt Vector Executed
//	when Input Capture Event Occured
}
*/

// Initialization
void TIMER1_init (uint8_t channelPort){
	TIMER1_OC_DDR	|=	 (1 << channelPort);
	TIMER1_OC_PORT	&=	~(1 << channelPort);
	maxDuty = 0xFFFF;
	sei();
}

void TIMER1_stop (){
	TIMER1_setClockMode (TIMER1_CLOCK_SELECT_NO_SOURCE);
}

uint16_t TIMER1_getCountValue (){
	return TCNT1;
}

// Set PWM Modes
void TIMER1_setPwm20Khz (uint8_t channel){
	TIMER1_setWaveGenerationMode		(TIMER1_WGM_MODE_F_PWM_ICR);
	TIMER1_setCompareOutputMode			(channel, TIMER1_COM_MODE_NON_INVERT);
	TIMER1_setClockMode					(TIMER1_CLOCK_SELECT_PRE_1);
	TIMER1_setInputCaptureReg			(TIMER1_F_PWM_20K_PRE1_ICR);
	// maxDuty = TIMER1_F_PWM_20K_PRE1_ICR;
}

void TIMER1_setPwm10Khz (uint8_t channel){
	TIMER1_setWaveGenerationMode		(TIMER1_WGM_MODE_F_PWM_ICR);
	TIMER1_setCompareOutputMode			(channel, TIMER1_COM_MODE_NON_INVERT);
	TIMER1_setClockMode					(TIMER1_CLOCK_SELECT_PRE_1);
	TIMER1_setInputCaptureReg			(TIMER1_F_PWM_10K_PRE1_ICR);
	// maxDuty = TIMER1_F_PWM_10K_PRE1_ICR;
}

void TIMER1_setPwm (uint8_t channel, uint16_t frequency){
// 	For ULTRASONIC
	TIMER1_setWaveGenerationMode		(TIMER1_WGM_MODE_F_PWM_OCR);
	TIMER1_setClockMode					(TIMER1_CLOCK_SELECT_PRE_1);
	maxDuty = TIMER1_getPWMFrequencyValue(frequency);
	TIMER1_setCompareOutputMode			(TIMER1_CH_B, TIMER1_COM_MODE_NON_INVERT);
	TIMER1_setOutputCompareReg			(TIMER1_CH_A, maxDuty);
}

uint16_t TIMER1_getPWMFrequencyValue (uint16_t desiredFrequency){
// 	TOP + 1 = F_CPU / PRESCALER / PWM_FREQ
	return (F_CPU / (1 * desiredFrequency) - 1);
}

void TIMER1_setOneSecTick (uint8_t channel){
	TIMER1_setWaveGenerationMode		(TIMER1_WGM_MODE_CTC);
	TIMER1_setCompareOutputMode			(channel, TIMER1_COM_MODE_NORMAL);
	TIMER1_setClockMode					(TIMER1_CLOCK_SELECT_PRE_256);
	TIMER1_setOutputCompareReg			(channel, TIMER1_CTC_1_SEC_OCR_TH);
}

void TIMER1_setDuty (uint8_t channel, uint16_t stepValue){
	TIMER1_setOutputCompareReg(channel, stepValue);
}

// Set Wave Generation Mode
void TIMER1_setWaveGenerationMode (uint8_t modeNum){
	switch (modeNum){
		case TIMER1_WGM_MODE_NORMAL	:
// 			Normal
// 			TOP :	0xFFFF
			TCCR1B |= (0 << WGM13) | (0 << WGM12);
			TCCR1A |= (0 << WGM11) | (0 << WGM10);
		break;
		
		case TIMER1_WGM_MODE_PC_PWM_8	:
// 			PWM, Phase Correct, 8-bit
// 			TOP :	0x00FF
			TCCR1B |= (0 << WGM13) | (0 << WGM12);
			TCCR1A |= (0 << WGM11) | (1 << WGM10);
		break;
		
		case TIMER1_WGM_MODE_PC_PWM_9	:
// 			PWM, Phase Correct, 9-bit
// 			TOP :	0x01FF
			TCCR1B |= (0 << WGM13) | (0 << WGM12);
			TCCR1A |= (1 << WGM11) | (0 << WGM10);
		break;
	
		case TIMER1_WGM_MODE_PC_PWM_10	:
// 			PWM, Phase Correct, 10-bit
// 			TOP :	0x03FF
			TCCR1B |= (0 << WGM13) | (0 << WGM12);
			TCCR1A |= (1 << WGM11) | (1 << WGM10);
		break;
	
		case TIMER1_WGM_MODE_CTC	:
// 			CTC
// 			TOP :	OCR1A
			TCCR1B |= (0 << WGM13) | (1 << WGM12);
			TCCR1A |= (0 << WGM11) | (0 << WGM10);
		break;
	
		case TIMER1_WGM_MODE_F_PWM_8	:
// 			Fast PWM, 8-bit
// 			TOP :	0x00FF
			TCCR1B |= (0 << WGM13) | (1 << WGM12);
			TCCR1A |= (0 << WGM11) | (1 << WGM10);
		break;
	
		case TIMER1_WGM_MODE_F_PWM_9	:
// 			Fast PWM, 9-bit
// 			TOP :	0x01FF
			TCCR1B |= (0 << WGM13) | (1 << WGM12);
			TCCR1A |= (1 << WGM11) | (0 << WGM10);
		break;
	
		case TIMER1_WGM_MODE_F_PWM_10	:
// 			Fast PWM, 10-bit
// 			TOP :	0x03FF
			TCCR1B |= (0 << WGM13) | (1 << WGM12);
			TCCR1A |= (1 << WGM11) | (1 << WGM10);
		break;
		
		case TIMER1_WGM_MODE_PFC_PWM_ICR	:
// 			PWM, Phase and Frequency Correct
// 			TOP :	ICR1
			TCCR1B |= (1 << WGM13) | (0 << WGM12);
			TCCR1A |= (0 << WGM11) | (0 << WGM10);
		break;
	
		case TIMER1_WGM_MODE_PFC_PWM_OCR	:
// 			PWM, Phase and Frequency Correct
// 			TOP :	OCR1A
			TCCR1B |= (1 << WGM13) | (0 << WGM12);
			TCCR1A |= (0 << WGM11) | (1 << WGM10);
		break;
		
		case TIMER1_WGM_MODE_PC_PWM_ICR	:
// 			PWM, Phase Correct
// 			TOP :	ICR1
			TCCR1B |= (1 << WGM13) | (0 << WGM12);
			TCCR1A |= (1 << WGM11) | (0 << WGM10);
		break;
	
		case TIMER1_WGM_MODE_PC_PWM_OCR	:
// 			PWM, Phase Correct
// 			TOP :	OCR1A
			TCCR1B |= (1 << WGM13) | (0 << WGM12);
			TCCR1A |= (1 << WGM11) | (1 << WGM10);
		break;
	
		case TIMER1_WGM_MODE_CTC_ICR	:
// 			CTC
// 			TOP :	ICR1
			TCCR1B |= (1 << WGM13) | (1 << WGM12);
			TCCR1A |= (0 << WGM11) | (0 << WGM10);
		break;
	
		case TIMER1_WGM_MODE_F_PWM_ICR	:
// 			Fast PWM
// 			TOP :	ICR1
//			높은 주파수의 PWM 파형을 얻기 위해 주로 사용한다.
//			f_desired = (16,000,000 / prescale) / (TOP + 1)
//			20000 = (16,000,000 / 1) / (799 + 1)
//			위 수식에서, prescale을 1로 놓고 TOP값을 작은 ICR 상수로 놓을 수 있다.
//			이때 ICR 값의 n % 값을 갖는 OCR 값을 택하여 듀티비를 조절할 수 있다.
			TCCR1B |= (1 << WGM13) | (1 << WGM12);
			TCCR1A |= (1 << WGM11) | (0 << WGM10);
		break;
	
		case TIMER1_WGM_MODE_F_PWM_OCR	:
// 			Fast PWM
// 			TOP :	OCR1A
			TCCR1B |= (1 << WGM13) | (1 << WGM12);
			TCCR1A |= (1 << WGM11) | (1 << WGM10);
		break;
	}
}

// Set Compare Output Mode
void TIMER1_setCompareOutputMode (uint8_t channel, uint8_t modeNum){
	uint8_t channelOffset = 0x00;
	switch (channel){
		case TIMER1_CH_A :
			channelOffset = 4;
		break;
		
		case TIMER1_CH_B :
			channelOffset = 2;
		break;
		
		case TIMER1_CH_C :
			channelOffset = 0;
		break;
	}
	
	switch (modeNum){
		case TIMER1_COM_MODE_NORMAL		:
			TCCR1A |= (0 << (COM1C1 + channelOffset)) | (0 << (COM1C0 + channelOffset));
		break;
	
		case TIMER1_COM_MODE_TOGGLE		:
			TCCR1A |= (0 << (COM1C1 + channelOffset)) | (1 << (COM1C0 + channelOffset));
		break;
	
		case TIMER1_COM_MODE_NON_INVERT	:
			TCCR1A |= (1 << (COM1C1 + channelOffset)) | (0 << (COM1C0 + channelOffset));
		break;
	
		case TIMER1_COM_MODE_INVERT		:
			TCCR1A |= (1 << (COM1C1 + channelOffset)) | (1 << (COM1C0 + channelOffset));
		break;
	}
}

// Set Clock Mode
void TIMER1_setClockMode (uint8_t modeNum){
	switch (modeNum){
		case TIMER1_CLOCK_SELECT_NO_SOURCE	:
			TCCR1B |= (0 << CS12) | (0 << CS11) | (0 << CS10);	// No Clock Source
		break;
			
		case TIMER1_CLOCK_SELECT_PRE_1	:
			TCCR1B |= (0 << CS12) | (0 << CS11) | (1 << CS10); // clk / 1
		break;
		
		case TIMER1_CLOCK_SELECT_PRE_8	:
			TCCR1B |= (0 << CS12) | (1 << CS11) | (0 << CS10); // clk / 8
		break;
	
		case TIMER1_CLOCK_SELECT_PRE_64	:
			TCCR1B |= (0 << CS12) | (1 << CS11) | (1 << CS10); // clk / 64
		break;
	
		case TIMER1_CLOCK_SELECT_PRE_256	:
			TCCR1B |= (1 << CS12) | (0 << CS11) | (0 << CS10); // clk / 256
		break;
	
		case TIMER1_CLOCK_SELECT_PRE_1024	:
			TCCR1B |= (1 << CS12) | (0 << CS11) | (1 << CS10); // clk / 1024
		break;
	
		case TIMER1_CLOCK_SELECT_EXTERN_FALLING	:
			TCCR1B |= (1 << CS12) | (1 << CS11) | (0 << CS10); // External Clock Source T1 Pin, Clock on Falling Edge
		break;
	
		case TIMER1_CLOCK_SELECT_EXTERN_RISING	:
			TCCR1B |= (1 << CS12) | (1 << CS11) | (1 << CS10); // External Clock Source T1 Pin, Clock on Rising Edge
		break;
	}
}

// Enable Force Output Compare
void TIMER1_enableForceOutputCompare (uint8_t channel){
	switch (channel){
		case TIMER1_CH_A :
			TCCR1C |= (1 << FOC1A);
		break;
		
		case TIMER1_CH_B :
			TCCR1C |= (1 << FOC1B);
		break;
		
		case TIMER1_CH_C :
			TCCR1C |= (1 << FOC1C);
		break;
	}
}

// Enable Input Capture Interrupt
void TIMER1_enableInputCaptureInterrupt (){
	DDRD &= ~(1 << DDRD4);
	PORTD &= ~(1 << PORTD4);
	TIMSK |= (1 << TICIE1);
}

void TIMER1_disableInputCaptureInterrupt (){
	TIMSK &= ~(1 << TICIE1);	
}

// Enable Output Compare Interrupt
void TIMER1_enableOutputCompareInterrupt (uint8_t channel){
	switch (channel){
		case TIMER1_CH_A :
			TIMSK |= (1 << OCIE1A);
		break;
		
		case TIMER1_CH_B :
			TIMSK |= (1 << OCIE1B);
		break;
		
		case TIMER1_CH_C :
			ETIMSK |= (1 << OCIE1C);
		break;
	}
}

// Disable Output Compare Interrupt
void TIMER1_disableOutputCompareInterrupt (uint8_t channel){
	switch (channel){
		case TIMER1_CH_A :
			TIMSK &= ~(1 << OCIE1A);
		break;
		
		case TIMER1_CH_B :
			TIMSK &= ~(1 << OCIE1B);
		break;
		
		case TIMER1_CH_C :
			ETIMSK &= ~(1 << OCIE1C);
		break;
	}
}

// Enable Overflow Interrupt
void TIMER1_enableOverflowInterrupt (){
	TIMSK |= (1 << TOIE1);
}

// Disable Overflow Interrupt
void TIMER1_disableOverflowInterrupt (){
	TIMSK &= ~(1 << TOIE1);
}

// Output Compare Register Set
void TIMER1_setOutputCompareReg (uint8_t channel, uint16_t outputCompareThresold){
	// cli();
	switch (channel){
		case TIMER1_CH_A :
			OCR1A = outputCompareThresold;
		break;
		
		case TIMER1_CH_B :
			OCR1B = outputCompareThresold;
		break;
		
		case TIMER1_CH_C :
			OCR1C = outputCompareThresold;
		break;
	}
	// sei();
}

// Input Capture Register Set
void TIMER1_setInputCaptureReg (uint16_t inputCaptureThreshold){
	// cli();
	ICR1 = inputCaptureThreshold;
	// sei();
}

// Timer/Counter Register Set
void TIMER1_setTimerCounterReg (uint16_t count){
// 	cli();
	TCNT1 = count;
// 	sei();
}

// Input Capture Set
void TIMER1_setInputCaptureNoiseCanceler (){
	TCCR1B |= (1 << ICNC1);
}

void TIMER1_setInputCaptureEdge (uint8_t polarity){
	if (polarity == TIMER1_INPUT_CAPTURE_RISING_EDGE){
		TCCR1B |= (1 << ICES1);
	}
	else if (polarity == TIMER1_INPUT_CAPTURE_FALLING_EDGE){
		TCCR1B &= ~(1 << ICES1);
	}
}