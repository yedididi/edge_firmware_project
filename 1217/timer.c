#include "device_driver.h"

#define TIM2_FREQ 	  		(8000000.) 	      	// Hz
#define TIM2_TICK	  		(1000000./TIM2_FREQ)	// usec
#define TIME2_PLS_OF_1ms  	(1000./TIM2_TICK)
#define TIM2_MAX	  		(0xffffu)

#define TIM4_TICK	  		(20) 				// usec
#define TIM4_FREQ 	  		(1000000/TIM4_TICK) // Hz
#define TIME4_PLS_OF_1ms  	(1000/TIM4_TICK)
#define TIM4_MAX	  		(0xffffu)

void TIM2_Stopwatch_Start(void)
{
	Macro_Set_Bit(RCC->APB1ENR, 0);

	TIM2->CR1 = (1<<4)|(1<<3);
	TIM2->PSC = (unsigned int)(TIMXCLK/50000.0 + 0.5)-1;
	TIM2->ARR = TIM2_MAX;

	Macro_Set_Bit(TIM2->EGR,0);
	Macro_Set_Bit(TIM2->CR1, 0);
}

unsigned int TIM2_Stopwatch_Stop(void)
{
	unsigned int time;

	Macro_Clear_Bit(TIM2->CR1, 0);
	time = (TIM2_MAX - TIM2->CNT) * TIM2_TICK;
	return time;
}

/* Delay Time Max = 65536 * 20use = 1.3sec */

#if 0

void TIM2_Delay(int time)
{
	Macro_Set_Bit(RCC->APB1ENR, 0);

	TIM2->CR1 = (1<<4)|(1<<3);
	TIM2->PSC = (unsigned int)(TIMXCLK/(double)TIM2_FREQ + 0.5)-1;
	TIM2->ARR = TIME2_PLS_OF_1ms * time;

	Macro_Set_Bit(TIM2->EGR,0);
	Macro_Clear_Bit(TIM2->SR, 0);
	Macro_Set_Bit(TIM2->DIER, 0);
	Macro_Set_Bit(TIM2->CR1, 0);

	while(Macro_Check_Bit_Clear(TIM2->SR, 0));

	Macro_Clear_Bit(TIM2->CR1, 0);
	Macro_Clear_Bit(TIM2->DIER, 0);
}

#else

/* Delay Time Extended */

void TIM2_Delay(int time)
{
	int i;
	unsigned int t = TIME2_PLS_OF_1ms * time;

	Macro_Set_Bit(RCC->APB1ENR, 0);

	TIM2->PSC = (unsigned int)(TIMXCLK/(double)TIM2_FREQ + 0.5)-1;
	TIM2->CR1 = (1<<4)|(1<<3);
	TIM2->ARR = 0xffff;
	Macro_Set_Bit(TIM2->EGR,0);
	Macro_Set_Bit(TIM2->DIER, 0);

	for(i=0; i<(t/0xffffu); i++)
	{
		Macro_Set_Bit(TIM2->EGR,0);
		Macro_Clear_Bit(TIM2->SR, 0);
		Macro_Set_Bit(TIM2->CR1, 0);
		while(Macro_Check_Bit_Clear(TIM2->SR, 0));
	}

	TIM2->ARR = t % 0xffffu;
	Macro_Set_Bit(TIM2->EGR,0);
	Macro_Clear_Bit(TIM2->SR, 0);
	Macro_Set_Bit(TIM2->CR1, 0);
	while (Macro_Check_Bit_Clear(TIM2->SR, 0));

	Macro_Clear_Bit(TIM2->CR1, 0);
	Macro_Clear_Bit(TIM2->DIER, 0);
}

#endif

void TIM4_Repeat(int time)
{
	Macro_Set_Bit(RCC->APB1ENR, 2);

	TIM4->CR1 = (1<<4)|(0<<3);
	TIM4->PSC = (unsigned int)(TIMXCLK/(double)TIM4_FREQ + 0.5)-1;
	TIM4->ARR = TIME4_PLS_OF_1ms * time - 1;

	Macro_Set_Bit(TIM4->EGR,0);
	Macro_Clear_Bit(TIM4->SR, 0);
	Macro_Set_Bit(TIM4->DIER, 0);
	Macro_Set_Bit(TIM4->CR1, 0);
}

int TIM4_Check_Timeout(void)
{
	if(Macro_Check_Bit_Set(TIM4->SR, 0))
	{
		Macro_Clear_Bit(TIM4->SR, 0);
		return 1;
	}
	else
	{
		return 0;
	}
}

void TIM4_Stop(void)
{
	Macro_Clear_Bit(TIM4->CR1, 0);
	Macro_Clear_Bit(TIM4->DIER, 0);
}

void TIM4_Change_Value(int time)
{
	TIM4->ARR = TIME4_PLS_OF_1ms * time;
}

// #define TIM2_FREQ 	  		(8000000.) 	      	// Hz
// #define TIM2_TICK	  		(1000000./TIM2_FREQ)	// usec
// #define TIME2_PLS_OF_1ms  	(1000./TIM2_TICK)

// void TIM3_Out_Init(void)
// {
// 	//TIM3를 사용하려면 해당 타이머에 클럭이 공급되어야 한다. 따라서 TIM3 타이머의 클럭을 활성화한다.
// 	Macro_Set_Bit(RCC->APB1ENR, 1);

// 	//APB2ENR 레지스터는 APB2 버스의 주변 장치 클럭을 활성화하는 레지스터이다.
// 	//2번 비트를 활성화 시켰으므로 GPIOA의 클럭을 활성화한다.
// 	Macro_Set_Bit(RCC->APB2ENR, 2);

// 	//GPIOA->CRL 은 GPIOB의 핀 모드를 설정하는 레지스터이다.(113pg)
// 	//우리는 pa2, pa3번을 사용하므로 아래와 같이 두 줄로 0xb로 설정
// 	//0xb는 출력 속도가 50MHz인 대체 기능 푸시풀 모드(AF-PP) 라는뜻
// 	//즉, 아래 두 줄은 GPIOA 핀 2번, 3번을 TIM3의 PWM 출력 핀으로 설정함
// 	Macro_Write_Block(GPIOA->CRL,0xf,0xb,8);
// 	Macro_Write_Block(GPIOA->CRL,0xf,0xb,12);

// 	//TIM3->CCMR2는 TIM3의 채널 3, 4의 캡쳐/비교 모드 설정 레지스터이다.
// 	//이렇게 0x6으로 설정하면 PWM 모드 1로 설정이 된다
// 	//PWM 모드 1은 타이머 카운터가 ARR에 도달하면 핀이 LOW, CCR 값보다 작으면 HIGH
// 	Macro_Write_Block(TIM2->CCMR2,0x7,0x6,4);
// 	Macro_Write_Block(TIM2->CCMR2,0x7,0x6,12);

	
// 	//TIM3->CCER은 캡쳐/비교 출력 활성화 제어 레지스터이다
// 	//9번 비트(CC3P)를 0으로 설정하는 것은 PWM 출력의 극성을 기본값(Active High)로 설정
// 	//8번 비트(CC3E)를 1로 설정하는 것은 채널 3 출력 활성화
// 	//TIM3의 채널 3에서 PWM 신호 출력을 활성화하는 줄이다
// 	TIM2->CCER = (0<<9)|(1<<8)|(0<<13)|(1<<12);

// 	TIM2->CR1 = (1<<0)|(0<<3)|(0<<4);
// }

// void TIM3_Out_Freq_Generation(unsigned short freq)
// {
// 	// Timer 주파수가 TIM3_FREQ가 되도록 PSC 설정
// 	TIM2->PSC = (int)(TIMXCLK / TIM2_FREQ + 0.5) - 1;

// 	// 요청한 주파수가 되도록 ARR 설정
// 	TIM2->ARR = TIM2_FREQ / freq;

// 	// Duty Rate 50%가 되도록 CCR3 설정 arr/2 값 넣으면됨
// 	TIM2->CCR3 = TIM2->ARR / 2;
// 	TIM2->CCR4 = TIM2->ARR / 2;

// 	// Manual Update(UG 발생)
// 	Macro_Set_Bit(TIM2->EGR, 0);

// 	// // Down Counter, Repeat Mode, Timer Start
// 	// TIM3->CR1 = (1<<4) | (0<<3) | (1<<0);
// 	TIM2->CR1 = (1<<0)|(0<<3)|(0<<4);
// }

// void TIM3_Out_Stop(void)
// {
// 	Macro_Clear_Bit(TIM3->CR1, 0);
// 	Macro_Clear_Bit(TIM3->DIER, 0);
// }
