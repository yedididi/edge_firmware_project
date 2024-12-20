#include "device_driver.h"

int fanState = UARTCONTROL;
int speed = 5;
int motorState = STOP;

static void Sys_Init(void)
{
	Clock_Init();
	LED_Init();
	Uart_Init(115200);
	Key_Poll_Init();
    // Motor_Init();
    // TIM3_Out_Init();
    motor_pwm_init();
    Adc_Cds_Init();
}

void executeUartControl()
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
        control_motor(speed, motorState);
    }
}

void executeLightControl()
{
    volatile int i;

    Adc_Start();
    while(!Adc_Get_Status()) ;
    int adcData = Adc_Get_Data() % 10;
    control_motor(adcData, FORWARD);
    Uart1_Printf("0x%.4X\n", 460 + (40 * adcData));
    for(i=0; i<0x400000; i++);
}

void executeAutomatic()
{
    volatile int i;
    static int newSpeed = 0;

    newSpeed = (newSpeed + 1) % 10;
    control_motor(newSpeed, FORWARD);
    Uart1_Printf("0x%.4X\n", 460 + (40 * newSpeed));
    for(i=0; i<0x1000000; i++);
}

void Main(void)
{
    Sys_Init();
    Uart_Printf("<Motor project level 1>\n");

    while (1)
    {
        if (Key_Get_Pressed() == 1) //key0 pressed
        {
            Uart1_Printf("1. %d\n", fanState);
            if (fanState == UARTCONTROL)
                fanState = LIGHTCONTROL;
            else if (fanState == LIGHTCONTROL)
                fanState = UARTCONTROL;
            Key_Wait_Key_Released();
            Uart1_Printf("2. %d\n", fanState);
        }
        else if (Key_Get_Pressed() == 2) //key1 pressed
        {
            Uart1_Printf("1. %d\n", fanState);
            if (fanState == UARTCONTROL)
                fanState = AUTOMATIC;
            else if (fanState == AUTOMATIC)
                fanState = UARTCONTROL;            
            Key_Wait_Key_Released();
            Uart1_Printf("2. %d\n", fanState);
        }

        switch (fanState)
        {
            case (UARTCONTROL):
                Uart1_Printf("UARTCONTROL\n");
                executeUartControl();
                break;

            case (LIGHTCONTROL):
                Uart1_Printf("LIGHTCONTROL\n");
                executeLightControl();
                break;
            
            case (AUTOMATIC):
                Uart1_Printf("AUTOMATIC\n");
                executeAutomatic();
                break;
        }

        
    }
}