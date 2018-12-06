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
}
void writeLED(uint16_t data)
{
	GPIOE->ODR &= (0xFFFFEFFF);	//disables PE12
	GPIOE->ODR &= (0xFFFFDFFF);	//disables PE13
	GPIOE->ODR &= (0xFFFFBFFF);	//disables PE14
	GPIOE->ODR &= (0xFFFF7FFF);	//disables PE15
	if(data<= (0x720)) {	//lower
		GPIOE->ODR |= (0x1000);	//enables PE12 
	} else if(data<= (0x735)) {
		GPIOE->ODR |= (0x1000);	//enables PE12 
		GPIOE->ODR |= (0x2000);	//enables PE13 
	} else if(data<= (0x750)) {
		GPIOE->ODR |= (0x1000);	//enables PE12 
		GPIOE->ODR |= (0x2000);	//enables PE13 
		GPIOE->ODR |= (0x4000);	//enables PE14
	} else {
		GPIOE->ODR |= (0x1000);	//enables PE12 
		GPIOE->ODR |= (0x2000);	//enables PE13 
		GPIOE->ODR |= (0x4000);	//enables PE14
		GPIOE->ODR |= (0x8000);	//enables PE15
	}		
}
