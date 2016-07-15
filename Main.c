
/********************************
 * Header files
 ********************************
 */

#include "stm32f4xx.h" // Device header
#include <stdbool.h>   // for boolean types
#include "Power.h"     // defines how to startup power status checking / LED interfacing
#include "Audio.h"     // defines audio output and values
#include "LCD.h"       // defines interface methods for LCD screen
#include "Map.h"       // defines map variables/types/functions to change map
#include "Game.h"      // defines game rules and input handling

#warning "Don't forget to comment all these files"


Tile tiles[10];//used to store the tiles colors/patterns

//12 high x 16 wide
const int ref_map[map_height][map_width]=
{
	{0,4,0,2,0,0,0,0,0,4,3,3,0,0,0,2},
	{2,2,0,2,0,0,0,0,0,0,0,0,0,0,5,0},
	{0,0,0,2,2,2,6,2,0,0,0,0,0,0,0,0},
	{0,0,2,2,0,0,0,0,0,0,0,0,3,0,0,0},
	{0,0,2,2,0,0,0,0,2,2,0,3,3,3,0,0},
	{6,2,2,2,2,2,2,4,3,3,0,0,3,0,0,0},
	{0,8,0,0,0,4,2,0,4,3,3,0,0,0,0,0},
	{6,5,2,2,2,6,2,2,2,2,2,2,2,2,2,5},
	{3,4,3,0,2,0,8,3,3,3,0,0,8,5,8,0},
	{3,3,3,3,2,6,0,7,6,7,0,7,5,2,7,5},
	{3,0,0,3,2,6,5,2,6,8,6,0,0,7,0,5},
	{0,3,3,4,2,0,5,2,8,5,8,5,8,8,0,7}
};

int playerX,playerY;//holds player's place on map
int map[map_height][map_width];    //holds indices of map tiles

/*******************************
 * MAIN METHOD
 *******************************
 */

int main(void)
{
	int i,j;

	initAudio();
	init_power_status();

	Delay_init(); // Initialize Timers
	TFT_IO_init();// Initialize the I/O
	TFT_init(); 	// Initiallize the screen

	delay_ms(1000);

	//put opening screen here
	TFT_fillScreen(0xff, 0x33, 0x99);
	for(i=0;i<16;i++)
	{
		for(j=0;j<12;j++)
		{
			TFT_fill_area( (i*20),((i+1)*20),(j*20),((j+1)*20),0x000000);
		}
	}

	init_tiles();
	reset_map();
	game_button_init();


	while( !check_win() )
	{
		while( !check_user_input() ){}//wait until user has input something

		delay_ms(2);//wait one ms to debounce

		input_handler();//handle input
	}//while

	TFT_fillScreen(0x44, 0x77, 0xAA);
	return 0;
}//main

///////////////////////////////////////////////////////////////////////////////
//							INTERUPT HANDLERS  		                    	 //
///////////////////////////////////////////////////////////////////////////////

void TIM4_IRQHandler(void)
{
	NVIC_ClearPendingIRQ(TIM4_IRQn);

	if( (PWR->CSR & 4)==4 )
	{
		GPIOA->ODR &= ~(0x00000040);//toggle status of output bits
		GPIOA->ODR |=   0x00000080;//toggle status of output bits
	}//if power below threshhold, RED LED on
	else
	{
		GPIOA->ODR &= ~(0x00000080);//toggle status of output bits
		GPIOA->ODR |=   0x00000040;//toggle status of output bits
	}//if power above threshhold, green LED on

	TIM4->SR &= ~TIM_SR_UIF;//clearing flag
}//TIM4_IRQHandler

void TIM7_IRQHandler(void)
{
	#define delayLength 12
	#define speedDelay  64

	static int i = 0;
	static int j = 0;
	static int z = 0;
	static const int delays[delayLength] =
	{
		 270, 250, 230, 210, 210, 230, 250, 270, 250, 230, 250, 230
	};//array used to hold delay values

	NVIC_ClearPendingIRQ(TIM7_IRQn);

	TIM7->SR   &= ~TIM_SR_UIF;//clearing flag
	TIM7->CR1  |= 0x00000001; //re-enabling counting

	DAC->DHR12RD  = sineParts[i];//putting value in register
	DAC->SWTRIGR |= 0x00000001;  //outputting value in register

	if( i==(WAVE_RES-1) )
	{
		if(z==(speedDelay-1))
		{
			TIM7->ARR = delays[j];//getting next delay
			j=(j+1)%delayLength;  //incrementing j index
		}
		z=(z+1)%(speedDelay);
	}
	i = (i+1)%WAVE_RES;//incrementing i index
}//TIM7_IRQHandler

