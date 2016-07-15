#ifndef __LCD_H_
#define __LCD_H_

/*******************************
 * Defines
 *******************************
 */				
#define startOffset 27

/*******************************
 * Function Definitions
 *******************************
 */		
void TFT_IO_init(void);																												   
void TFT_init(void);																											  
void TFT_writeCMD(char CMD);																								  	 
void TFT_writeDATA(char DATA);																								  	 
void TFT_windowSet(int start_x, int end_x, int start_y, int end_y);																	     
void TFT_fillScreen(char RED, char GREEN, char BLUE);																			 
void TFT_fill_area(int start_x, int end_x, int start_y, int end_y, unsigned int color );
void TFT_fill_pixel(int x,int y, int color); 
void delay_ms(int ms);
void Delay_init(void);

#endif //__LCD_H_
