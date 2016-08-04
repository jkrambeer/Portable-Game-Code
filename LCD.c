/********************************
 * Header files 
 ********************************
 */
#include "stm32f4xx.h" 
#include "LCD.h"

/********************************
 * Delay Functions
 ********************************
 */

void Delay_init()
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
	TIM6->PSC = 15999;  //prescalar to divide 16MHz clock to 1KHz
}//Delay_init

void delay_ms(int ms)
{
	if(ms==0){return;}
	 
	TIM6->ARR = ms;     //set clock to count to how many ms you want delayed
	TIM6->CNT = 0;      //start count at 0
	TIM6->CR1|= TIM_CR1_CEN;//enable counting

	while(TIM6->SR==0);//wait until uIF flag is set
	TIM6->SR=0; //clear uIF flag
}//delay_ms

/********************************
 * Display Functions
 ********************************
 */

void TFT_IO_init()
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
	
	// Port D config
	GPIOD->MODER  |= 0x00005555;
	GPIOD->OTYPER &= ~0x000000FF;
	GPIOD->PUPDR  &= ~0x0000FFFF;
	
	// Port E config
	GPIOE->MODER  |= 0x00000155;
	GPIOE->OTYPER &= ~0x0000001F;
	GPIOE->PUPDR  &= ~0x000003FF;
}//TFT_IO_init

void TFT_init()
{
	
	GPIOE->ODR &= ~0x00000008;		// CS LOW
	GPIOE->ODR |= 0x00000004;		// RD HIGH
	GPIOE->ODR &= ~0x00000000;		// WR LOW
	
	GPIOE->ODR &= ~0x00000010;		// RES LOW (start hard reset)
	delay_ms(5);
	delay_ms(10);
	
	GPIOE->ODR |= 0x00000010;		// RES HIGH (finish hard reset)
	delay_ms(100);
	
	delay_ms(10);
	TFT_writeCMD(0x01);				// soft_reset
	delay_ms(10);
	
	delay_ms(10);
	TFT_writeCMD(0x01);				// soft_reset
	delay_ms(10);
	
	// Start and Lock PLL
	delay_ms(10);
	TFT_writeCMD(0xE0);
	TFT_writeDATA(0x01);
	TFT_writeCMD(0xE0);
	TFT_writeDATA(0x01);
	
	// Configure settings (datasheet page 44)
	TFT_writeCMD(0xB0);
	TFT_writeDATA(0x08);
	TFT_writeDATA(0x80);
	TFT_writeDATA(0x01);
	TFT_writeDATA(0xDF);
	TFT_writeDATA(0x01);
	TFT_writeDATA(0x0F);
	TFT_writeDATA(0x00);
	
	// More settings (datasheet page 78
	TFT_writeCMD(0xF0);
	TFT_writeDATA(0x00);
		
	// More settings (datasheet page 76)
	TFT_writeCMD(0xE6);
	TFT_writeDATA(0x01);
	TFT_writeDATA(0x45);
	TFT_writeDATA(0x47);
	
	// More settings (datasheet page 47)
	TFT_writeCMD(0xB4);
	TFT_writeDATA(0x02);
	TFT_writeDATA(0x0D);
	TFT_writeDATA(0x00);
	TFT_writeDATA(0x2B);
	TFT_writeDATA(0x28);
	TFT_writeDATA(0x00);
	TFT_writeDATA(0x00);
	TFT_writeDATA(0x00);
	
	// More settings (datasheet page 49)
	TFT_writeCMD(0xB6);
	TFT_writeDATA(0x01);
	TFT_writeDATA(0x1d);
	TFT_writeDATA(0x00);
	TFT_writeDATA(0x0C);
	TFT_writeDATA(0x09);
	TFT_writeDATA(0x00);
	TFT_writeDATA(0x00);
	
	// More settings (datasheet page 30)
	TFT_writeCMD(0x2A);
	TFT_writeDATA(0x00);
	TFT_writeDATA(0x00);
	TFT_writeDATA(0x01);
	TFT_writeDATA(0xDF);
	
	// More settings (datasheet page 30)
	TFT_writeCMD(0x2B);
	TFT_writeDATA(0x00);
	TFT_writeDATA(0x00);
	TFT_writeDATA(0x01);
	TFT_writeDATA(0x0F);
	
	// More settings (datasheet page 27)
	TFT_writeCMD(0x13);
	
	// More settings (datasheet page 40)
	TFT_writeCMD(0x38);
	
	// More settings (datasheet page 29)
	TFT_writeCMD(0x29);
	
}//TFT_init

void TFT_writeCMD(char COMMAND)
{
	GPIOE->ODR &= ~0x00000008;	// CS LOW
	GPIOE->ODR &= ~0x00000001;	// Bring RS LOW
	GPIOE->ODR &= ~0x00000002;	// Bring WR LOW

	GPIOD->ODR = COMMAND;		// Fill port D with COMMAND
	
	GPIOE->ODR |= 0x00000008;	// CS HIGH
	GPIOE->ODR |= 0x00000002;	// Bring WR HIGH
	GPIOE->ODR |= 0x00000001;	// Bring RS HIGH
}//TFT_writeCMD

void TFT_writeDATA(char DATA)
{
	GPIOE->ODR &= ~0x00000008;	// CS LOW
	GPIOE->ODR |= 0x00000001;	// Bring RS HIGH
	GPIOE->ODR &= ~0x00000002;	// Bring WR LOW

	GPIOD->ODR = DATA;		// Fill port D with DATA
	
	GPIOE->ODR |= 0x00000008;	// CS HIGH
	GPIOE->ODR |= 0x00000002;	// Bring WR HIGH
}//TFT_writeDATA

void TFT_windowSet(int start_x, int end_x, int start_y, int end_y)
{
	TFT_writeCMD(0x2A);
	TFT_writeDATA((start_x)>>8);
	TFT_writeDATA(start_x);
	TFT_writeDATA((end_x)>>8);
	TFT_writeDATA(end_x);
	
	
	TFT_writeCMD(0x2B);
	TFT_writeDATA((start_y)>>8);
	TFT_writeDATA(start_y);
	TFT_writeDATA((end_y)>>8);
	TFT_writeDATA(end_y);
}//TFT_windowSet

void TFT_fill_pixel(int x,int y, int color)
{
	TFT_fill_area(x,x,y,y,color);
}//TFT_fill_pixel

void TFT_fill_area(int start_x, int end_x, int start_y, int end_y, unsigned int color )
{
	int i;
	char RED   = (color>>16)&0xFF; 
	char GREEN = (color>>8) &0xFF;
	char BLUE  = (color>>0) &0xFF;

	TFT_windowSet(start_x+startOffset,end_x+startOffset,start_y,end_y);
	TFT_writeCMD(0x2C);
	for(i=0; i<(end_x-start_x+1)*(end_y-start_y+1); i++)
	{
		TFT_writeDATA(RED);
		TFT_writeDATA(GREEN);
		TFT_writeDATA(BLUE);	
	}//for i
}//TFT_fill_area

void TFT_fillScreen(char RED, char GREEN, char BLUE)
{
	int i;
	TFT_windowSet(0x0000, 0x01DF, 0x0000, 0x010F);
	
	TFT_writeCMD(0x2C);
	for(i=0; i<130560; i++)
	{
		TFT_writeDATA(RED);
		TFT_writeDATA(GREEN);
		TFT_writeDATA(BLUE);	
	}//for i
}//TFT_fillScreen
