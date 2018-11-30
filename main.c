#include "ADCcontroller.h"
#include "LEDController.h"
#include "EffectsController.h"
#include "DACcontroller.h"
#include "stm32l4xx.h"
#include "systemInit.h"
#include "lcd.h"
#include <stdbool.h>

uint16_t data[7505] = {0};
uint16_t dataIndex=0;

void EXTI2_IRQHandler(void) {
LCD_DisplayString((uint8_t*)"");
LCD_DisplayString((uint8_t*)"three");
EXTI->PR1 |= EXTI_PR1_PIF2;
}
void EXTI1_IRQHandler(void) {
LCD_DisplayString((uint8_t*)"");
LCD_DisplayString((uint8_t*)"one");
EXTI->PR1 |= EXTI_PR1_PIF1;
}
void EXTI0_IRQHandler(void) {
LCD_DisplayString((uint8_t*)"");
LCD_DisplayString((uint8_t*)"two");
EXTI->PR1 |= EXTI_PR1_PIF0;
}

void SysTick_Handler(void)  {                              
	data[dataIndex]=readADC();	//store ADC val in data
	dataIndex++;	//increment dataIndex (post index)
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
LCD_Initialization();
SysTick->CTRL |= 1; //enable sysTick
	
while(1)
{}

}
