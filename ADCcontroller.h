#ifndef ADCCONTROLLER_H
#define ADCCONTROLLER_H
#include "stm32l4xx.h"                  // Device header

extern uint16_t loopArray[7500];
void initADC(void);
void ADC1_Wakeup(void);
void readADC();
void SysTick_Initialize();

	
#endif
