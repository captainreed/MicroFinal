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
bool loop_playing = false;
bool loop_recording = false;
bool overdrive_active = false;
bool delay_active = false;
int recording_index = 0;
int playback_index = 0;

//left most button
void EXTI3_IRQHandler(void) {
if(loop_recording){
	loop_recording = false;
	LCD_DisplayString((uint8_t*)"      ");
	LCD_DisplayString((uint8_t*)"rec 0");
}
else{
	loop_recording = true;
	LCD_DisplayString((uint8_t*)"      ");
	LCD_DisplayString((uint8_t*)"rec 1");
}
EXTI->PR1 |= EXTI_PR1_PIF1;
}
//center button
void EXTI0_IRQHandler(void) {
if(loop_playing){
	loop_playing = false;
	LCD_DisplayString((uint8_t*)"      ");
	LCD_DisplayString((uint8_t*)"loop0");
}
else{
	loop_playing = true;
	LCD_DisplayString((uint8_t*)"      ");
	LCD_DisplayString((uint8_t*)"loop1");
}
EXTI->PR1 |= EXTI_PR1_PIF0;
}
//right most button
void EXTI2_IRQHandler(void) {	
if(overdrive_active){
overdrive_active = false;
LCD_DisplayString((uint8_t*)"      ");
LCD_DisplayString((uint8_t*)"OD 0");//OverDrive
}
else{
overdrive_active = true;
LCD_DisplayString((uint8_t*)"      ");
LCD_DisplayString((uint8_t*)"OD 1");//OverDrive
}
	
EXTI->PR1 |= EXTI_PR1_PIF2;
}

void SysTick_Handler(void)  {                              
	if(loop_recording)
	{
	readADC();	//store ADC val in data
	if(loop_playing){playback_index = recording_index;}
	if(recording_index == samplesPerLoop){loop_recording = false;}
	}
	if(loop_playing)
	{
	handleEffects(data, overdrive_active, delay_active, recording_index, playback_index);
	writeDAC(data[playback_index]);	
	writeLED();	
	playback_index++;
	playback_index = playback_index%samplesPerLoop;
	}
}

void ADC1_2_IRQHandler(void) {
	data[recording_index]=((ADC1->DR)&(0x0000FFFF));
	recording_index++;	//increment dataIndex (post index)
	recording_index=recording_index%samplesPerLoop;
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
