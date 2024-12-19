#include "device_driver.h"

void Motor_Init() 
{
    // 1. RCC 클럭 활성화 (GPIOA 활성화)
    RCC->APB2ENR |= (1 << 2); // RCC_APB2ENR_IOPAEN (GPIOA 클럭 활성화)

    // 2. GPIOA CRL 레지스터 설정 (PA2, PA3 출력 모드 설정)
    GPIOA->CRL &= ~((0xF << 8) | (0xF << 12)); // PA2, PA3 비트 초기화
    GPIOA->CRL |= ((0x3 << 8) | (0x3 << 12));  // PA2, PA3를 출력 모드(50MHz, Push-Pull)로 설정

    // 3. 초기 출력값을 Low로 설정
    GPIOA->ODR &= ~((1 << 2) | (1 << 3)); // PA2, PA3 비트를 0으로 설정 (Low)
}

void Motor_Forward(int speed)
{
    volatile int i;
    for (i = 0; i < 0x40000; i++) ;

    // 모터 정방향 (PA1=1, PA2=0)
    // GPIOA->ODR |= (1 << 2);  // PA2 HIGH
    // GPIOA->ODR &= ~(1 << 3); // PA3 LOW

    TIM3_Out_Freq_Generation(speed);
    TIM3_Out_Stop();
    //Uart_Printf("F entered\n");
}

void Motor_Reverse(int speed)
{
    volatile int i;
    // 모터 역방향 (PA1=1, PA2=0)
    for (i = 0; i < 0x40000; i++) ;

    // GPIOA->ODR &= ~(1 << 2);  // PA2 LOW
    // GPIOA->ODR |= (1 << 3);  // PA3 HIGH

    TIM3_Out_Freq_Generation(speed);
    TIM3_Out_Stop();
    //Uart_Printf("R entered\n");
}

void Motor_Stop()
{
    volatile int i;
    // 모터 정지 (PA1=1, PA2=0)
    for (i = 0; i < 0x40000; i++) ;
    // GPIOA->ODR |= (1 << 2);  // PA2 HIGH
    // GPIOA->ODR |= (1 << 3); // PA3 LOW

    TIM3_Out_Freq_Generation(0);
    TIM3_Out_Stop();
    //.Uart_Printf("S entered\n");
}