#include "device_driver.h"

enum _motorState {STOP, FORWARD, REVERSE};
int speed = 1;
int motorState = STOP;

static void Sys_Init(void)
{
	Clock_Init();
	LED_Init();
	Uart_Init(115200);
	Key_Poll_Init();
    //Motor_Init();
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
                    motorState = FORWARD;
                else if (USART1->DR == 'R')
                    motorState = REVERSE;
                else if (USART1->DR == 'S')
                    motorState = STOP;
                else if (USART1->DR >= '1' && USART1->DR <= '9')
                    speed = USART1->DR - '0';
                break;
            }

            switch (motorState)
            {
                case STOP:
                    Motor_Stop();
                    break;
             
                case FORWARD:
                    Motor_Forward(speed);
                    break;
             
                case REVERSE:
                    Motor_Reverse(speed);
            }
        }
    }
}