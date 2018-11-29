#include "ADCcontroller.h"
#include "LEDController.h"
#include "EffectsController.h"
#include "DACcontroller.h"
#include "stm32l4xx.h"
#include "systemInit.h"
#include <stdbool.h>

void writeDAC()
{
	
		
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
