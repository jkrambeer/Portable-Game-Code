/********************************
 * Header files 
 ********************************
 */
#include "stm32f4xx.h"
#include "Power.h"

void init_power_status(void)
{
	
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;//enabling PWR's clock
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;//enabling TIM4's clock
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;//enabling port A's clock
	
	PWR->CR |= 0x000000D0;//setting PLS to 110
	
	//PA6 - controls green LED
	//PA7 - controls red LED
	GPIOA->MODER |= 0x00005000;//set PA6 and PA7 as outputs
	GPIOA->ODR   |= 0x00000040;//set PA6 output high to power green LED
	
	TIM4->PSC  = 15999;  //no prescalar 
	TIM4->ARR  = 999;    //auto reload register
	
	TIM4->DIER |= 0x00000001;     //setting interrupt bit enabled
	TIM4->CR1  |= 0x00000001;     //enabling counting on timer
	
	
	__enable_irq();
	NVIC_SetPriority(TIM4_IRQn,0);
	NVIC_ClearPendingIRQ(TIM4_IRQn);
	NVIC_EnableIRQ(TIM4_IRQn);
}//init_power_status
