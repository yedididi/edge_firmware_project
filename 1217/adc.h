#ifndef ADC_H
#define ADC_H

#include "adc.h"

void Adc_Cds_Init(void);
void Adc_Start(void);
void Adc_Stop(void);
int Adc_Get_Status(void);
int Adc_Get_Data(void);

#endif