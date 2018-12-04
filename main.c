#include "ADCcontroller.h"
#include "LEDController.h"
#include "EffectsController.h"
#include "DACcontroller.h"
#include "stm32l4xx.h"
#include "systemInit.h"
#include "lcd.h"
#include <stdbool.h>

#define samplesPerLoop 7505

uint16_t data[samplesPerLoop] = {0};
uint16_t dataIndex=0;
extern bool playBackEnabled;
bool recording = false;

//left most button
void EXTI1_IRQHandler(void) {
LCD_DisplayString((uint8_t*)"      ");
LCD_DisplayString((uint8_t*)"record");	
if(recording){
	recording = false;
}
else{
	recording = true;
}
EXTI->PR1 |= EXTI_PR1_PIF1;
}
//middle button
void EXTI0_IRQHandler(void) {
LCD_DisplayString((uint8_t*)"      ");
LCD_DisplayString((uint8_t*)"loop");
if(playBackEnabled){
	playBackEnabled = false;
}
else{
	playBackEnabled = true;
}
EXTI->PR1 |= EXTI_PR1_PIF0;
}
//right most button
void EXTI2_IRQHandler(void) {
LCD_DisplayString((uint8_t*)"      ");
LCD_DisplayString((uint8_t*)"OD");//OverDrive
EXTI->PR1 |= EXTI_PR1_PIF2;
}

void SysTick_Handler(void)  {                              
	readADC();	//store ADC val in data
	if(playBackEnabled)
	{
	handleEffects();
	writeDAC(data[dataIndex]);	
	writeLED();	
	}
	dataIndex++;	//increment dataIndex (post index)
	dataIndex=dataIndex%samplesPerLoop;
}

int main(void){
playBackEnabled = false;
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
