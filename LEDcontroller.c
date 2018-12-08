#include "ADCcontroller.h"
#include "LEDController.h"
#include "EffectsController.h"
#include "DACcontroller.h"
#include "stm32l4xx.h"
#include "systemInit.h"
#include <stdbool.h>


void initLED()//initialize the LED pins
{
	RCC->AHB2ENR |=RCC_AHB2ENR_GPIOEEN; 
	GPIOE->MODER &= (0x00FFFFFF);	//sets GPIO pins 12, 13, 14, 15 to output mode
	GPIOE->MODER |= (0x55000000);
	GPIOE->PUPDR &= (0x00FFFFFF);	//sets pull-down resistors for 12, 13, 14, 15
	GPIOE->PUPDR |= (0xAA000000);	
	//play a little light show to confirm system is on
	
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
	
	
}
void writeLED(uint16_t data)
{
	if(data==0) {
	GPIOE->ODR |= (0x1000);	//enables PE12 
	}
	if(data==1) {
		GPIOE->ODR |= (0x2000);	//enables PE13 
	}
	if(data==2) {
		
		GPIOE->ODR |= (0x4000);	//enables PE14
	}
	if(data==3) {
		GPIOE->ODR |= (0x8000);	//enables PE15
	}
	if(data==7) {
		GPIOE->ODR &= (0xFFFF7FFF);	//disables PE15
	}
	if(data==4) {
		GPIOE->ODR &= (0xFFFFEFFF);	//disables PE12
	}
	if(data==5) {
		GPIOE->ODR &= (0xFFFFDFFF);	//disables PE13
	}
	if(data==6) {
		GPIOE->ODR &= (0xFFFFBFFF);	//disables PE14
	}
			
}
