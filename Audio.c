/********************************
 * Header files 
 ********************************
 */
#include "stm32f4xx.h" 
#include "Audio.h"     // defines audio output and values

const unsigned short sineParts[WAVE_RES] = 
{ 
		2048, 2145, 2242, 2339, 2435, 2530, 2624, 2717, 2808, 2897, 
    2984, 3069, 3151, 3230, 3307, 3381, 3451, 3518, 3581, 3640, 
    3696, 3748, 3795, 3838, 3877, 3911, 3941, 3966, 3986, 4002, 
    4013, 4019, 4020, 4016, 4008, 3995, 3977, 3954, 3926, 3894, 
    3858, 3817, 3772, 3722, 3669, 3611, 3550, 3485, 3416, 3344, 
    3269, 3191, 3110, 3027, 2941, 2853, 2763, 2671, 2578, 2483, 
    2387, 2291, 2194, 2096, 1999, 1901, 1804, 1708, 1612, 1517, 
    1424, 1332, 1242, 1154, 1068, 985,  904,  826,  751,  679, 
    610,  545,  484,  426,  373,  323,  278,  237,  201,  169, 
    141,  118,  100,  87,   79,   75,   76,   82,   93,   109, 
    129,  154,  184,  218,  257,  300,  347,  399,  455,  514, 
    577,  644,  714,  788,  865,  944,  1026, 1111, 1198, 1287, 
    1378, 1471, 1565, 1660, 1756, 1853, 1950, 2047 
};//sign wave of amplitude broken into 128 parts 

void initAudio(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;//enabling port A's clock
	RCC->APB1ENR |= RCC_APB1ENR_DACEN;  //enabling DAC's clock
	RCC->APB1ENR |= RCC_APB1ENR_TIM7EN; //enabling TIM7's clock

	GPIOA->MODER  |= 0x00000100; //putting PA4 in analog mode
	
	//all of these are for DAC channel 1
	DAC->CR |= 0x00000004;//DAC trigger enable
	DAC->CR |= 0x00000038;//DAC select software event
	DAC->CR |= 0x00000002;//no buffered output (might wanna change)
	DAC->CR |= 0x00000001;//enable DAC
	
	//TIM7 Setup
	TIM7->PSC  = 0;  //no prescalar 
	TIM7->ARR  = 300;//auto reload register
	TIM7->DIER |= 0x00000001;     //setting interrupt bit enabled
	TIM7->CR1  |= 0x00000008;     //One pulse mode
	TIM7->CR1  |= 0x00000001;     //enabling counting on timer
	
	__enable_irq();//enabling interrupts
	NVIC_SetPriority(TIM7_IRQn,1);
	NVIC_ClearPendingIRQ(TIM7_IRQn);
	NVIC_EnableIRQ(TIM7_IRQn);
	 
}//init audio
