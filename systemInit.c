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
	GPIOA->MODER &=(0xFFFFFFC0);    //sets GPIO A pins 0, 1, and 2 for input mode
	GPIOA->PUPDR &=(0xFFFFFFC0);    //sets GPIO A pins 0, 1, and 2 to pull-down mode
  GPIOA->PUPDR |=(0x0000002A);
	GPIOA->MODER &=(0xFFFFF3FF);    //sets GPIO A pin 5 to output mode
	GPIOA->MODER |=(0x00000400);
	NVIC_EnableIRQ(EXTI0_IRQn);     //enables EXTI0 interrupt
	NVIC_EnableIRQ(EXTI1_IRQn);     //enables EXTI1 interrupt
	NVIC_EnableIRQ(EXTI2_IRQn);     //enables EXTI2 interrupt
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;   //enables SYSCFG
	SYSCFG->EXTICR[0] &= (0xFFFFF000);      //ties interrupts to GPIO A
	EXTI->RTSR1 |= (0x00000007);    //enables rising trigger selection for interrupt 0, 1, and 2
	EXTI->IMR1 |= (0x00000007);     //enables interrupt mask register for interrupt 0, 1, and 2
		
}

void sysTick_Initialize() {
SysTick->CTRL = 0;
SysTick->LOAD = 8000-1;//set the systick to go off every 2 ms
NVIC_SetPriority(SysTick_IRQn, (1<<__NVIC_PRIO_BITS)-1);
SysTick->VAL=0;
SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk;
}