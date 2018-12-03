#include "ADCcontroller.h"
#include "LEDController.h"
#include "EffectsController.h"
#include "DACcontroller.h"
#include "stm32l4xx.h"
#include "systemInit.h"
#include <stdbool.h>

void writeDAC(uint16_t val)
{
//this is where we will write to the dac
	while((DAC->SR & DAC_SR_BWST2) !=0);
	DAC->DHR12R2 = val;  //write data to DAC output
	DAC->SWTRIGR |= DAC_SWTRIGR_SWTRIG2;    //triggers DAC output
	for(int i =0; i <=10; i++);     //small delay
}

void DAC_initialize(void) {
	RCC->APB1ENR1 |= RCC_APB1ENR1_DAC1EN;
	DAC->CR &= ~(DAC_CR_EN1 | DAC_CR_EN2);
	DAC->MCR &= ~(7U<<16);
	DAC->CR |= DAC_CR_TEN2;
	DAC->CR |= DAC_CR_TSEL2;
	DAC->CR |= DAC_CR_EN2;
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	GPIOA->MODER |= 3U<<(2*5); 
}
