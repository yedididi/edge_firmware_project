#include "pwm.h"

#define TIM2_FREQ 	  		(8000000.) 	      	// Hz
#define TIM2_TICK	  		(1000000./TIM2_FREQ)	// usec
#define TIME2_PLS_OF_1ms  	(1000./TIM2_TICK)

void TIM3_Out_Init(void)
{
	//TIM3를 사용하려면 해당 타이머에 클럭이 공급되어야 한다. 따라서 TIM3 타이머의 클럭을 활성화한다.
	Macro_Set_Bit(RCC->APB1ENR, 1);

	//APB2ENR 레지스터는 APB2 버스의 주변 장치 클럭을 활성화하는 레지스터이다.
	//2번 비트를 활성화 시켰으므로 GPIOA의 클럭을 활성화한다.
	Macro_Set_Bit(RCC->APB2ENR, 2);

	//GPIOA->CRL 은 GPIOB의 핀 모드를 설정하는 레지스터이다.(113pg)
	//우리는 pa2, pa3번을 사용하므로 아래와 같이 두 줄로 0xb로 설정
	//0xb는 출력 속도가 50MHz인 대체 기능 푸시풀 모드(AF-PP) 라는뜻
	//즉, 아래 두 줄은 GPIOA 핀 2번, 3번을 TIM3의 PWM 출력 핀으로 설정함
	Macro_Write_Block(GPIOA->CRL,0xf,0xb,8);
	Macro_Write_Block(GPIOA->CRL,0xf,0xb,12);

	//TIM3->CCMR2는 TIM3의 채널 3, 4의 캡쳐/비교 모드 설정 레지스터이다.
	//이렇게 0x6으로 설정하면 PWM 모드 1로 설정이 된다
	//PWM 모드 1은 타이머 카운터가 ARR에 도달하면 핀이 LOW, CCR 값보다 작으면 HIGH
	Macro_Write_Block(TIM2->CCMR2,0x7,0x6,4);
	Macro_Write_Block(TIM2->CCMR2,0x7,0x6,12);

	
	//TIM3->CCER은 캡쳐/비교 출력 활성화 제어 레지스터이다
	//9번 비트(CC3P)를 0으로 설정하는 것은 PWM 출력의 극성을 기본값(Active High)로 설정
	//8번 비트(CC3E)를 1로 설정하는 것은 채널 3 출력 활성화
	//TIM3의 채널 3에서 PWM 신호 출력을 활성화하는 줄이다
	TIM2->CCER = (0<<9)|(1<<8)|(0<<13)|(1<<12);

	//TIM2->CR1 = (1<<0)|(0<<3)|(0<<4);
}

void TIM3_Out_Freq_Generation(int speed)
{
	// Timer 주파수가 TIM3_FREQ가 되도록 PSC 설정
	TIM2->PSC = (int)(TIMXCLK / TIM2_FREQ + 0.5) - 1;

	// 요청한 주파수가 되도록 ARR 설정
	TIM2->ARR = TIM2_FREQ;

	// Duty Rate 50%가 되도록 CCR3 설정 arr/2 값 넣으면됨
	//motor 하나는 양, 다른건 음으로 넣어야 하는데 이렇게 같은 값을 줘도 되나??
	TIM2->CCR3 = TIM2->ARR / (10 - speed);
	//TIM2->CCR4 = TIM2->ARR / 2;

	// Manual Update(UG 발생)
	Macro_Set_Bit(TIM2->EGR, 0);

	// // Down Counter, Repeat Mode, Timer Start
	// TIM3->CR1 = (1<<4) | (0<<3) | (1<<0);
	TIM2->CR1 = (1<<0)|(0<<3)|(0<<4);
}

void TIM3_Out_Stop(void)
{
	Macro_Clear_Bit(TIM3->CR1, 0);
	Macro_Clear_Bit(TIM3->DIER, 0);
}
