#include "device_driver.h"

static void Sys_Init(void)
{
	Clock_Init();
	LED_Init();
	Uart_Init(115200);
	Key_Poll_Init();
    Motor_Init();
    TIM3_Out_Init();
}

void Main(void)
{
    Sys_Init();
    Uart_Printf("<Motor project level 1>\n");

    for (;;)
    {
        while (1)
        {
            if (Macro_Check_Bit_Set(USART1->SR,5))
            {
                Uart_Printf("%c\n", USART1->DR);
                if (USART1->DR == 'F')
                    Motor_Forward();
                else if (USART1->DR == 'R')
                    Motor_Reverse();
                else if (USART1->DR == 'S')
                    Motor_Stop();
                break;
            }
        }
    }
}