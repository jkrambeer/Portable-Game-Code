#include <stdbool.h>   // for boolean types

#ifndef __GAME_H_
#define __GAME_H_

/********************************
 * Type Definitions 
 ********************************
 */
typedef enum _buttons { primary   = 0x0080, 
											  secondary = 0x0100, 
												down      = 0x0200, 
												left      = 0x0400, 
												up        = 0x0800, 
												right     = 0x1000 } Button;

//values that would be high in an IDR for
//PE7-PE12 to use as bit masks to decide
//which button was pressed												
												
/*******************************
 * Function Defintions
 *******************************
 */		
bool check_win(void);
void apply_tile_effects(Button direction);										
bool check_legal_move(Button direction);
void input_handler(void);
void break_blocks(void);
bool check_user_input(void);											
void game_button_init(void);	
bool check_win(void);												
												
#endif //__MAP_H_
