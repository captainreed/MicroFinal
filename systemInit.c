#include "ADCcontroller.h"
#include "LEDController.h"
#include "EffectsController.h"
#include "DACcontroller.h"
#include "stm32l4xx.h"
#include "systemInit.h"
#include <stdbool.h>

void configureInterrupts()
{
RCC->AHB2ENR |=RCC_AHB2ENR_GPIOAEN;     //enables GPIO_A clock
RCC->AHB2ENR |=RCC_AHB2ENR_GPIOBEN;     //enables GPIO_A clock
RCC->AHB2ENR |=RCC_AHB2ENR_GPIOEEN;     //enables GPIO_A clock
	
	GPIOA->MODER &=(0xFFFFFF0C);    //sets GPIO A pins 0, 2, and 3 for input mode
	GPIOA->PUPDR &=(0xFFFFFF0C);    //sets GPIO A pins 0, 2, and 3 to pull-down mode
  GPIOA->PUPDR |=(0x000000A2);
	
	GPIOB->MODER &=(0xFFFF0FFF);    //sets GPIO B pins 6 and 7 for input mode
	GPIOB->PUPDR &=(0xFFFF0FFF);    //sets GPIO B pins 6 and 7 to pull-down mode
  GPIOB->PUPDR |=(0x0000A000);
	
	GPIOE->MODER &=(0xFFCFFFFF);    //sets GPIO E pin 10 for input mode
	GPIOE->PUPDR &=(0xFFCFFFFF);    //sets GPIO E pins 10 to pull-down mode
  GPIOE->PUPDR |=(0x00200000);
	
	NVIC_EnableIRQ(EXTI0_IRQn);     //enables EXTI0 interrupt
	NVIC_EnableIRQ(EXTI3_IRQn);     //enables EXTI3 interrupt
	NVIC_EnableIRQ(EXTI2_IRQn);     //enables EXTI2 interrupt
	NVIC_EnableIRQ(EXTI9_5_IRQn);   //enables EXTI9_5 interrupt for using pb6
	NVIC_EnableIRQ(EXTI15_10_IRQn); //enables EXTI15_10 interrupt for using pe10
		
	
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;   //enables SYSCFG
	SYSCFG->EXTICR[0] &= (0xFFFF00F0);      //ties interrupts to GPIO A
	SYSCFG->EXTICR[2] &= (0xFFFF00F0);      //ties interrupts to GPIO B
	SYSCFG->EXTICR[3] &= (0xFFFF4FF);      //ties interrupts to GPIO E
	EXTI->RTSR1 |= (0x000004CC);    //enables rising trigger selection for interrupt 0, 1, and 2
	EXTI->IMR1 |= (0x000004CC);     //enables interrupt mask register for interrupt 0, 1, and 2
		
}

void sysTick_Initialize() {
SysTick->CTRL = 0;
SysTick->LOAD = 1000-1;//set the systick to go off every 2 ms
NVIC_SetPriority(SysTick_IRQn, (1<<__NVIC_PRIO_BITS)-1);
SysTick->VAL=0;
SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk;
}
