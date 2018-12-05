#include "ADCcontroller.h"
#include "LEDController.h"
#include "EffectsController.h"
#include "DACcontroller.h"
#include "stm32l4xx.h"
#include "systemInit.h"
#include "lcd.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define samplesPerLoop 960000

uint16_t data[samplesPerLoop] = {0};
bool loop_playing = false;
bool loop_recording = false;
bool overdrive_active = false;
bool delay_active = false;
bool record_pending = false;
int recording_index = 0;
int playback_index = 0;

char operationString[6];
char timeNumberChar[2];

int display_time = 0;

//left most button
void EXTI3_IRQHandler(void) {
if(loop_recording){
	loop_recording = false;
	LCD_Clear();
	LCD_DisplayString((uint8_t*)"record");
}
else{
	//record_pending = true;
	loop_recording = true;
	LCD_Clear();
	LCD_DisplayString((uint8_t*)"rec 1");
}
EXTI->PR1 |= EXTI_PR1_PIF3;
}
//center button
void EXTI0_IRQHandler(void) {
if(loop_playing){
	loop_playing = false;
	LCD_Clear();
	LCD_DisplayString((uint8_t*)"loop0");
}
else{
	loop_playing = true;
	LCD_Clear();
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
if(playback_index%64000 == 0){
//sprintf(timeNumberChar, "%d", display_time);
}
if(record_pending && loop_playing && playback_index ==0)
{
loop_recording = true;
record_pending = false;
}	
	if(loop_recording)
	{
	readADC();	//store ADC val in data
	if(loop_playing){playback_index = recording_index;}
	if(recording_index == samplesPerLoop){loop_recording = false; loop_playing=true;}
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
	if(loop_playing){
	data[recording_index]+=((ADC1->DR)&(0x0000FFFF));
	}
	else{
	data[recording_index]=((ADC1->DR)&(0x0000FFFF));
	}
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
