/********************************
 * Header files
 ********************************
 */

#include "stm32f4xx.h"
#include "Game.h"
#include "Map.h"
#include "LCD.h" //for delay function

//button check order:
//PE7  - primary button
//PE8  - secondary button
//PE9  - down
//PE10 - left
//PE11 - up
//PE12 - right

bool check_win(void)
{
	int i,j;
	for(i=0; i<map_height; i++)
	{
		for(j=0; j<map_width; j++)
		{
			//if any movable tile isnt visited return false
			if( (map[i][j]==breakableBlock) || (map[i][j]==unvisitedTile) ){return false;}
		}//for j
	}//for i
	return true;//all tiles that could be visited were visited so they won
}//check_win

bool check_user_input(void)
{
	const int mask = primary | secondary | down | left | up | right;//create mask to only check input values
	return (( GPIOE->IDR & mask )>0);//check if at least one input value is high in the IDR
}//check_user_input

void game_button_init(void)
{
	//setting PE7-12 as inputs
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
	GPIOE->MODER &= 0xFC003FFF;//setting PE7-12 as inputs
	GPIOE->PUPDR |= 0x02AA8000;
}//game_button_init

bool check_legal_move(Button direction)
{
	int x = playerX;
	int y = playerY;

	//checking for illegal moves, moving off screen or into breakable block
	if(direction==down)
	{
		if( (playerY == map_height-1) || (map[y+1][x]==breakableBlock)){return false;}
	}
	else if(direction==left)
	{
		if( (playerX == 0) || (map[y][x-1]==breakableBlock) ){return false;}
	}
	else if(direction==up)
	{
		if( (playerY == 0) || (map[y-1][x]==breakableBlock)){return false;}
	}
	else if(direction==right)
	{
		if( (playerX == map_width-1) || (map[y][x+1]==breakableBlock)){return false;}
	}
	return true;
}//check_legal_move

void input_handler(void)
{
	if( (GPIOE->IDR & primary ) == primary )
	{
		break_blocks();
	}//primary
	else if( (GPIOE->IDR & secondary) == secondary )
	{
		reset_map();
	}//secondary
	else if( (GPIOE->IDR & down) == down )
	{
		if(!check_legal_move(down) ){return;}//illegal move
		playerY++;
		apply_tile_effects(down);
	}//down
	else if( (GPIOE->IDR & left) == left )
	{
		if(!check_legal_move(left) ){return;}//illegal move
		playerX--;
		apply_tile_effects(left);
	}//left
	else if( (GPIOE->IDR & up) == up )
	{
		if(!check_legal_move(up) ){return;}//illegal move
		playerY--;
		apply_tile_effects(up);
	}//up
	else if( (GPIOE->IDR & right ) == right)
	{
		if(!check_legal_move(right) ){return;}//illegal move, can't move off mapp
		playerX++;
		apply_tile_effects(right);
	}//right

	//wait until all input lines are dropped to allow only one input per button press
	while( check_user_input() ){}

	delay_ms(10);//delay on ms to debounce end of signal
	local_refresh();

}//input_handler

void break_blocks(void)
{
	int x = playerX;
	int y = playerY;

	//these check if the tile exists and if it a breakable tile before they change a tile
	if( (playerY!=map_height-1)   && (map[y+1][x]==breakableBlock) ){ map[y+1][x]=unvisitedTile; }//down
	if( (playerX!=0)              && (map[y][x-1]==breakableBlock) ){ map[y][x-1]=unvisitedTile; }//left
	if( (playerY!=0)              && (map[y-1][x]==breakableBlock) ){ map[y-1][x]=unvisitedTile; }//up
	if( (playerX!=map_width-1)    && (map[y][x+1]==breakableBlock) ){ map[y][x+1]=unvisitedTile; }//right
}//break_blocks

void apply_tile_effects(Button direction)
{
	int  x,y;
	bool change;
	int  currentTile;

	do
	{
		x = playerX;
		y = playerY;
		currentTile = map[y][x];
		change = false;

		local_refresh();

		if(currentTile==ice)
		{
			if     (direction==down) {currentTile=downTile; }
			else if(direction==left) {currentTile=leftTile; }
			else if(direction==up)   {currentTile=upTile;   }
			else if(direction==right){currentTile=rightTile;}
			change = true;
		}//ice

		if(currentTile==visitedTile || currentTile==magma)
		{
			reset_map();//they re-moved on a tile or fell into lava, restart game
		}
		else if(currentTile==unvisitedTile)
		{
			map[y][x]=visitedTile;
		}
		else if(currentTile==downTile)
		{
			if( !check_legal_move(down) ){return;}
			playerY++;
			change    = true;
			direction = down;
		}//down directional tile
		else if(currentTile==leftTile)
		{
			if( !check_legal_move(left) ){return;}
			playerX--;
			change    = true;
			direction = left;
		}//left directional tile
		else if(currentTile==upTile)
		{
			if( !check_legal_move(up) ){return;}
			playerY--;
			change    = true;
			direction = up;
		}//up directional tile
		else if(currentTile==rightTile)
		{
			if( !check_legal_move(right) ){return;}
			playerX++;
			change    = true;
			direction = right;
		}//right directional tile

	} while( change );//do while no changes
}//apply_tile_effects

