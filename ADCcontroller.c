#include "ADCcontroller.h"
#include "LEDController.h"
#include "EffectsController.h"
#include "DACcontroller.h"
#include "stm32l4xx.h"
#include "systemInit.h"
#include <stdbool.h>

void initADC()//initialize the ADC
{
	RCC->CR |= RCC_CR_HSION;    //turns HSI16 on
	while((RCC->CR & RCC_CR_HSIRDY) == 0);  //waits till HSI16 is ready
	GPIOA->MODER |= (0xC); //sets PA.1 to analog mode
	GPIOA->PUPDR |= (0x8);	//sets pull-down resistor for PA.1
	GPIOA->ASCR |= (0x2); //conects PA.1 to ADC
	RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN;	//step 1
	ADC1->CR &=(0xE);	//step 2
	SYSCFG->CFGR1 |= SYSCFG_CFGR1_BOOSTEN;	//step 3
	ADC123_COMMON->CCR |= (0x400000);	//step 4
	ADC123_COMMON->CCR &= (0xFFC3FFFF);	//step 5
	ADC123_COMMON->CCR &=(0xFFFCFFFF);	//step 6
	ADC123_COMMON->CCR |=(0x10000);	//step 6
	ADC123_COMMON->CCR &= (0xFFFFFFE0); //step 7
	ADC1_Wakeup(); //check function, compare to book
	ADC1->CFGR &= (0xFFFFFFC7);	//step 9 and 10
	ADC1->SQR1 &=(0xFFFFFFF0); //step 11
	ADC1->SQR1 |=(0x180); //step 12
	ADC1->DIFSEL &= (0xFFFFFFBF);	//step 13
	ADC1->SMPR1 &= (0xFFE3FFFF);	//step 14
	ADC1->SMPR1 |= (0x80000);	//step 14
	ADC1->CFGR &= (0xFFFFDFFF);	//step 15
	ADC1->CFGR &= (0xFFFFF3FF); //step 16
	ADC1->IER |= (0x4); //enables EOC interrupt
	NVIC_EnableIRQ(ADC1_2_IRQn);     //enables EXTI2 interrupt
	ADC1->CR |= (0x00000001); //step 17
	while((ADC1->CR&0x00000001)!=0x00000001);	
}

void readADC()
{
	ADC1->CR |= (0x00000004);
}

void ADC1_Wakeup(void) {
int wait_time;
if((ADC1->CR & ADC_CR_DEEPPWD) == ADC_CR_DEEPPWD) {
ADC1->CR &= ~ADC_CR_DEEPPWD;
}
ADC1->CR |= ADC_CR_ADVREGEN;
wait_time=20*(80);
while(wait_time != 0) {
wait_time--;
}
}
