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

#define samplesPerLoop 48000
#define ODCutOffValue 0x0750
#define debounceVal 20000

uint16_t data[samplesPerLoop] = {0};
bool loop_playing = false;
bool loop_recording = false;
bool overdrive_active = false;
bool delay_active = false;
bool record_pending = false;
int recording_index = 0;
int playback_index = 0;
uint16_t LEDcode=0;
uint16_t buffer=0;
void debounce(int duration) {
	for(int i=0; i<duration; i++) {
		(void)0;
	}
}

char operationString[6];
char timeNumberChar[2];

int display_time = 0;

//delay button
void EXTI9_5_IRQHandler(void)
{
	debounce(debounceVal);
	//record_pending = true;
	loop_recording = true;
	LCD_Clear();
	LCD_DisplayString((uint8_t*)"rec1");

EXTI->PR1 |= EXTI_PR1_PIF3;
}


//record button
void EXTI3_IRQHandler(void) {
	debounce(debounceVal);
	loop_recording = true;
	LCD_Clear();
	LCD_DisplayString((uint8_t*)"rec1");
EXTI->PR1 |= EXTI_PR1_PIF3;
}

//Loop Button
void EXTI0_IRQHandler(void) {
	debounce(debounceVal);
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

//clear button
void EXTI15_10_IRQHandler(void) {
	for(int i=0; i<samplesPerLoop; i++) {
		data[i]=0;
	}
	bool loop_playing = false;
	bool loop_recording = false;
	bool overdrive_active = false;
	bool delay_active = false;
	bool record_pending = false;
	int recording_index = 0;
	int playback_index = 0;
	uint16_t LEDcode=0;
	uint16_t buffer=0;
	EXTI->PR1 |= EXTI_PR1_PIF10;
}

//overdrive button
void EXTI2_IRQHandler(void) {	
		debounce(debounceVal);
if(overdrive_active){
overdrive_active = false;
LCD_Clear();
LCD_DisplayString((uint8_t*)"OD0");//OverDrive
}
else{
overdrive_active = true;
LCD_Clear();
LCD_DisplayString((uint8_t*)"OD1");//OverDrive
}
EXTI->PR1 |= EXTI_PR1_PIF2;
}

void SysTick_Handler(void)  {
if(loop_recording)
	{
		readADC();	//store ADC val in data
		if(loop_playing){playback_index = recording_index;}
		
		if(recording_index == samplesPerLoop-1){
		loop_recording = false; 
		loop_playing=true;
		LCD_Clear();
		LCD_DisplayString((uint8_t*)"loop1");
		}
		if(buffer >= 100) {
			buffer=0;
			LEDcode=(LEDcode+7)%8;
			writeLED(LEDcode);
		}
		buffer++;
	}
	if(loop_playing)
	{
		if(delay_active)
			{
				if(playback_index >= 4000){data[playback_index-4000] = data[playback_index];}
				else{data[samplesPerLoop - playback_index -  4000] = data[playback_index];}
			}
			
		if(overdrive_active)
		{
			if(data[playback_index] > ODCutOffValue){
				writeDAC(ODCutOffValue);
			}
			if(loop_recording) {
			//do nothing
		} else {
			if(buffer >= 50) {
				buffer=0;
				LEDcode=(LEDcode+1)%8;
				writeLED(LEDcode);
			}
			buffer++;
		}
		} else {
			writeDAC(data[playback_index]);
		if(loop_recording) {
			//do nothing
		} else {
			if(buffer >= 200) {
				buffer=0;
				LEDcode=(LEDcode+1)%8;
				writeLED(LEDcode);
			}
			buffer++;
		}
		}
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
