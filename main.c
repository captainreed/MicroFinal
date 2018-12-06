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

uint16_t data[samplesPerLoop] = {0};
bool loop_playing = false;
bool loop_recording = false;
bool overdrive_active = false;
bool delay_active = false;
bool record_pending = false;
int recording_index = 0;
int playback_index = 0;
uint16_t dummy=0;
uint32_t averageLED=0;

char operationString[6];
char timeNumberChar[2];

int display_time = 0;

//top button
void EXTI3_IRQHandler(void) {
if(loop_recording){
	loop_recording = false;
	LCD_Clear();
	LCD_DisplayString((uint8_t*)"rec0");
}
else{
	//record_pending = true;
	loop_recording = true;
	LCD_Clear();
	LCD_DisplayString((uint8_t*)"rec1");
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
//if(playback_index%4000 == 0){
////sprintf(timeNumberChar, "%d", display_time);
//}
//if(record_pending && playback_index ==0)
//{
//loop_recording = true;
//record_pending = false;
//}	
//if(loop_recording && recording_index == 0){
//loop_recording = false;
//record_pending = false;
//}
if(loop_recording)
{
readADC();	//store ADC val in data
if(loop_playing){playback_index = recording_index;}
	if(recording_index == samplesPerLoop){
	loop_recording = false; 
	loop_playing=true;
	LCD_Clear();
	LCD_DisplayString((uint8_t*)"loop1");
	}
	if(loop_playing)
	{
	handleEffects(data, overdrive_active, delay_active, recording_index, playback_index);
	if(dummy<1000) {
		averageLED+=data[playback_index];
		
	} else {
		averageLED/=1000;
		writeLED(averageLED);
	}
	dummy++;
	writeDAC(data[playback_index]);	
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
	int a=0;
GPIOE->ODR |= (0x1000);	//enables PE12 
	for(int i=0; i<15000; i++) {
		a++;
	}
	a=0;
	GPIOE->ODR |= (0x2000);	//enables PE13 
	for(int i=0; i<15000; i++) {
		a++;
	}
	a=0;
		GPIOE->ODR |= (0x4000);	//enables PE14
	for(int i=0; i<15000; i++) {
		a++;
	}
	a=0;
		GPIOE->ODR |= (0x8000);	//enables PE15
	for(int i=0; i<15000; i++) {
		a++;
	}
		a=0;
		GPIOE->ODR &= (0xFFFFEFFF);	//disables PE12
	for(int i=0; i<15000; i++) {
		a++;
	}
	a=0;
		GPIOE->ODR &= (0xFFFFDFFF);	//disables PE13
	for(int i=0; i<15000; i++) {
		a++;
	}
	a=0;
		GPIOE->ODR &= (0xFFFFBFFF);	//disables PE14
	for(int i=0; i<15000; i++) {
		a++;
	}
	a=0;
		GPIOE->ODR &= (0xFFFF7FFF);	//disables PE15
	for(int i=0; i<15000; i++) {
		a++;
	}
	SysTick->CTRL |= 1; //enable sysTick
while(1)
{}
}
