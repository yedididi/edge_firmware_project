#include "device_driver.h"

static void Sys_Init(void)
{
	Clock_Init();
	LED_Init();
	Uart_Init(115200);
	Key_Poll_Init();
    //Motor_Init();
    //TIM3_Out_Init();
    motor_pwm_init();
}

void Main(void)
{
    int speed = 1;
    Sys_Init();
    Uart_Printf("<Motor project level 1>\n");

    while (1)
    {
        if (Macro_Check_Bit_Set(USART1->SR,5))
        {
            Uart_Printf("%c\n", USART1->DR);
            if (USART1->DR == 'F')
                control_motor(speed, FORWARD);
            else if (USART1->DR == 'R')
                control_motor(speed, REVERSE);
            else if (USART1->DR == 'S')
                control_motor(speed, STOP);
            else if (USART1->DR >= '1' && USART1->DR <= '9')
                speed = USART1->DR - '0';
            break;
        }
    }
}