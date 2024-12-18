#include "pwm.h"

void TIM3_Out_Init(void)
{
	Macro_Set_Bit(RCC->APB1ENR, 1);
	Macro_Set_Bit(RCC->APB2ENR, 3);
	Macro_Write_Block(GPIOB->CRL,0xf,0xb,0);
	Macro_Write_Block(TIM3->CCMR2,0x7,0x6,4);
	TIM3->CCER = (0<<9)|(1<<8);
}


void TIM3_Out_Freq_Generation(unsigned short freq)
{
	// Timer 주파수가 TIM3_FREQ가 되도록 PSC 설정
	TIM3->PSC = (int)(TIMXCLK / TIM3_FREQ + 0.5) - 1;

	// 요청한 주파수가 되도록 ARR 설정
	TIM3->ARR = TIM3_FREQ / freq;

	// Duty Rate 50%가 되도록 CCR3 설정 arr/2 값 넣으면됨
	TIM3->CCR3 = TIM3->ARR / 2;

	// Manual Update(UG 발생)
	Macro_Set_Bit(TIM3->EGR, 0);

	// Down Counter, Repeat Mode, Timer Start
	TIM3->CR1 = (1<<4) | (0<<3) | (1<<0);
}

void TIM3_Out_Stop(void)
{
	Macro_Clear_Bit(TIM3->CR1, 0);
	Macro_Clear_Bit(TIM3->DIER, 0);
}
