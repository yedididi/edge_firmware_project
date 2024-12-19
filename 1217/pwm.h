#ifndef PWM_H
#define PWM_H

#define TIM3_FREQ 	  		(8000000.) 	      	// Hz
#define TIM3_TICK	  		(1000000./TIM3_FREQ)	// usec
#define TIME3_PLS_OF_1ms  	(1000./TIM3_TICK)

#include "device_driver.h"

void TIM3_Out_Init(void);
void TIM3_Out_Freq_Generation(int speed);
void TIM3_Out_Stop(void);


#endif