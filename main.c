#include "ADCcontroller.h"
#include "LEDController.h"
#include "EffectsController.h"
#include "DACcontroller.h"
#include "stm32l4xx.h"
#include "systemInit.h"
#include <stdbool.h>

bool playBackEnabled = false;

void EXTI2_IRQHandler(void) {

EXTI->PR1 |= EXTI_PR1_PIF2;
}
void EXTI1_IRQHandler(void) {
	
EXTI->PR1 |= EXTI_PR1_PIF1;
}
void EXTI0_IRQHandler(void) {
EXTI->PR1 |= EXTI_PR1_PIF0;
}

void SysTick_Handler(void)  {                              
readADC();
	if(playBackEnabled)
	{
	handleEffects();
	writeDAC();	
	writeLED();	
	}
}

int main(void){
configureInterrupts();
sysTick_Initialize();
initADC();
DAC_initialize();
initEffects();
initLED();
}
