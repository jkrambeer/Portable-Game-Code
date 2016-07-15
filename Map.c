/********************************
 * Header files
 ********************************
 */
#include "stm32f4xx.h"
#include <string.h>    // for memcpy
#include "LCD.h"
#include "Map.h"

void reset_map(void)
{
	//resetting player coordinates
	playerX = startX;
	playerY = startY;

	//resetting map to initial state by copying ref map to map
	memcpy(map, ref_map, sizeof(ref_map) );

	//setting the player start position as visited
	map[playerY][playerX]=visitedTile;

	//showing the map
	refresh_map();
}//reset_state

void refresh_map(void)
{
	int y,x;
	for(y=0;y<12;y++)
	{
		for(x=0;x<16;x++)
		{
			if(playerY==y && playerX==x){write_tile(tiles[characterTile],x*20,y*20);}
			else                        {write_tile(tiles[  map[y][x]  ],x*20,y*20);}
		}//for y
	}//for x
}//refresh

void local_refresh(void)
{
	int x,y;
	x = playerX;
	y = playerY;

	if(x != 0)                               {write_tile(tiles[ map[y][x-1]   ],(x-1)*20,(y+0)*20);}//left
	if(y != 0)                               {write_tile(tiles[ map[y-1][x]   ],(x+0)*20,(y-1)*20);}//up
	if(x != map_width-1)                     {write_tile(tiles[ map[y][x+1]   ],(x+1)*20,(y+0)*20);}//right
	if(y != map_height-1)                    {write_tile(tiles[ map[y+1][x]   ],(x+0)*20,(y+1)*20);}//down
	if(x !=0 && y != 0)                      {write_tile(tiles[ map[y-1][x-1] ],(x-1)*20,(y-1)*20);}//upper left
	if(x != map_width-1 && y != 0)           {write_tile(tiles[ map[y-1][x+1] ],(x+1)*20,(y-1)*20);}//upper right
	if(x !=0 && y != map_height-1)           {write_tile(tiles[ map[y+1][x-1] ],(x-1)*20,(y+1)*20);}//lower left
	if(x != map_width-1 && y != map_height-1){write_tile(tiles[ map[y+1][x+1] ],(x+1)*20,(y+1)*20);}//lower right

	write_tile(tiles[characterTile],x*20,y*20);//unconditional tile refresh of character's tile
}//local_refresh

void write_tile(Tile tile, int start_x, int start_y)
{
	int i,j,index;
	for(i=0;i<20;i++)
	{
		for(j=0;j<20;j++)
		{
			index = ( (tile.pattern[i]>>(j))&1 );
			TFT_fill_pixel(start_x+i,start_y+j,tile.colors[index]);
		}//for i
	}//for j
}//write_tile

void init_tiles(void)
{
	int x;

	//format is:
	//1). fill color
	//2). detail color
	//3). design pattern 0-19 (20bits x 20 indices)

	x = unvisitedTile;//index is 0, solid black tile
	tiles[x].colors[0]   = 0x0d0d0d;//fill color  : light black
	tiles[x].colors[1]   = 0x0d0d0d;//detail color: light black
	tiles[x].pattern[0]  = 0x00000;
	tiles[x].pattern[1]  = 0x00000;
	tiles[x].pattern[2]  = 0x00000;
	tiles[x].pattern[3]  = 0x00000;
	tiles[x].pattern[4]  = 0x00000;
	tiles[x].pattern[5]  = 0x00000;
	tiles[x].pattern[6]  = 0x00000;
	tiles[x].pattern[7]  = 0x00000;
	tiles[x].pattern[8]  = 0x00000;
	tiles[x].pattern[9]  = 0x00000;
	tiles[x].pattern[10] = 0x00000;
	tiles[x].pattern[11] = 0x00000;
	tiles[x].pattern[12] = 0x00000;
	tiles[x].pattern[13] = 0x00000;
	tiles[x].pattern[14] = 0x00000;
	tiles[x].pattern[15] = 0x00000;
	tiles[x].pattern[16] = 0x00000;
	tiles[x].pattern[17] = 0x00000;
	tiles[x].pattern[18] = 0x00000;
	tiles[x].pattern[19] = 0x00000;

	x = visitedTile;//index 1, black tile with light green x
	tiles[x].colors[0]   = 0x0d0d0d;//fill color  : light black
	tiles[x].colors[1]   = 0xcc00cc;//detail color: magenta
	tiles[x].pattern[0]  = 0x80001;
	tiles[x].pattern[1]  = 0x40002;
	tiles[x].pattern[2]  = 0x20004;
	tiles[x].pattern[3]  = 0x10008;
	tiles[x].pattern[4]  = 0x08010;
	tiles[x].pattern[5]  = 0x04020;
	tiles[x].pattern[6]  = 0x02040;
	tiles[x].pattern[7]  = 0x01080;
	tiles[x].pattern[8]  = 0x00900;
	tiles[x].pattern[9]  = 0x00600;
	tiles[x].pattern[10] = 0x00600;
	tiles[x].pattern[11] = 0x00900;
	tiles[x].pattern[12] = 0x01080;
	tiles[x].pattern[13] = 0x02040;
	tiles[x].pattern[14] = 0x04020;
	tiles[x].pattern[15] = 0x08010;
	tiles[x].pattern[16] = 0x10008;
	tiles[x].pattern[17] = 0x20004;
	tiles[x].pattern[18] = 0x40002;
	tiles[x].pattern[19] = 0x80001;

	x = magma;
	tiles[x].colors[0]   = 0xff0000;//fill color  : red
	tiles[x].colors[1]   = 0xffff00;//detail color: yellow
	tiles[x].pattern[0]  = 0x00000;
	tiles[x].pattern[1]  = 0x11004;
	tiles[x].pattern[2]  = 0x20822;
	tiles[x].pattern[3]  = 0x40440;
	tiles[x].pattern[4]  = 0x02200;
	tiles[x].pattern[5]  = 0x04010;
	tiles[x].pattern[6]  = 0x20820;
	tiles[x].pattern[7]  = 0x10448;
	tiles[x].pattern[8]  = 0x08204;
	tiles[x].pattern[9]  = 0x00002;
	tiles[x].pattern[10] = 0x00840;
	tiles[x].pattern[11] = 0x21020;
	tiles[x].pattern[12] = 0x42010;
	tiles[x].pattern[13] = 0x00102;
	tiles[x].pattern[14] = 0x00204;
	tiles[x].pattern[15] = 0x10440;
	tiles[x].pattern[16] = 0x22020;
	tiles[x].pattern[17] = 0x41010;
	tiles[x].pattern[18] = 0x00008;
	tiles[x].pattern[19] = 0x00000;

	x = ice;
	tiles[x].colors[0]   = 0x0099ff;//fill color  : aqua
	tiles[x].colors[1]   = 0xFFFFFF;//detail color: grey
	tiles[x].pattern[0]  = 0x00000;
	tiles[x].pattern[1]  = 0x08004;
	tiles[x].pattern[2]  = 0x04802;
	tiles[x].pattern[3]  = 0x00400;
	tiles[x].pattern[4]  = 0x40200;
	tiles[x].pattern[5]  = 0x20000;
	tiles[x].pattern[6]  = 0x10000;
	tiles[x].pattern[7]  = 0x00020;
	tiles[x].pattern[8]  = 0x00810;
	tiles[x].pattern[9]  = 0x00400;
	tiles[x].pattern[10] = 0x00200;
	tiles[x].pattern[11] = 0x40000;
	tiles[x].pattern[12] = 0x20000;
	tiles[x].pattern[13] = 0x02008;
	tiles[x].pattern[14] = 0x01004;
	tiles[x].pattern[15] = 0x00802;
	tiles[x].pattern[16] = 0x40400;
	tiles[x].pattern[17] = 0x20000;
	tiles[x].pattern[18] = 0x10020;
	tiles[x].pattern[19] = 0x00010;

	x = breakableBlock;
	tiles[x].colors[0]   = 0x663300;//fill color  : brown
	tiles[x].colors[1]   = 0x2d2d1f;//detail color: grey
	tiles[x].pattern[0]  = 0xFFFFF;
	tiles[x].pattern[1]  = 0xC0003;
	tiles[x].pattern[2]  = 0xA0005;
	tiles[x].pattern[3]  = 0x90009;
	tiles[x].pattern[4]  = 0x88011;
	tiles[x].pattern[5]  = 0x84021;
	tiles[x].pattern[6]  = 0x82041;
	tiles[x].pattern[7]  = 0x81081;
	tiles[x].pattern[8]  = 0x80901;
	tiles[x].pattern[9]  = 0x80601;
	tiles[x].pattern[10] = 0x80601;
	tiles[x].pattern[11] = 0x80901;
	tiles[x].pattern[12] = 0x81081;
	tiles[x].pattern[13] = 0x82041;
	tiles[x].pattern[14] = 0x84021;
	tiles[x].pattern[15] = 0x88011;
	tiles[x].pattern[16] = 0x90009;
	tiles[x].pattern[17] = 0xA0005;
	tiles[x].pattern[18] = 0xC0003;
	tiles[x].pattern[19] = 0xFFFFF;

	x = upTile;
	tiles[x].colors[0]   = 0x0d0d0d;//fill color  : light black
	tiles[x].colors[1]   = 0xff9900;//detail color: gold
	tiles[x].pattern[0]  = 0x00000;
	tiles[x].pattern[1]  = 0x00000;
	tiles[x].pattern[2]  = 0x00000;
	tiles[x].pattern[3]  = 0x00000;
	tiles[x].pattern[4]  = 0x00000;
	tiles[x].pattern[5]  = 0x00010;
	tiles[x].pattern[6]  = 0x00018;
	tiles[x].pattern[7]  = 0x0001C;
	tiles[x].pattern[8]  = 0xFFFFE;
	tiles[x].pattern[9]  = 0xFFFFF;
	tiles[x].pattern[10] = 0xFFFFF;
	tiles[x].pattern[11] = 0xFFFFE;
	tiles[x].pattern[12] = 0x0001C;
	tiles[x].pattern[13] = 0x00018;
	tiles[x].pattern[14] = 0x00010;
	tiles[x].pattern[15] = 0x00000;
	tiles[x].pattern[16] = 0x00000;
	tiles[x].pattern[17] = 0x00000;
	tiles[x].pattern[18] = 0x00000;
	tiles[x].pattern[19] = 0x00000;

	x = downTile;
	tiles[x].colors[0]   = 0x0d0d0d;//fill color  : light black
	tiles[x].colors[1]   = 0x0033cc;//detail color: blue
	tiles[x].pattern[0]  = 0x00000;
	tiles[x].pattern[1]  = 0x00000;
	tiles[x].pattern[2]  = 0x00000;
	tiles[x].pattern[3]  = 0x00000;
	tiles[x].pattern[4]  = 0x00000;
	tiles[x].pattern[5]  = 0x08000;
	tiles[x].pattern[6]  = 0x18000;
	tiles[x].pattern[7]  = 0x38000;
	tiles[x].pattern[8]  = 0x7FFFF;
	tiles[x].pattern[9]  = 0xFFFFF;
	tiles[x].pattern[10] = 0xFFFFF;
	tiles[x].pattern[11] = 0x7FFFF;
	tiles[x].pattern[12] = 0x38000;
	tiles[x].pattern[13] = 0x18000;
	tiles[x].pattern[14] = 0x08000;
	tiles[x].pattern[15] = 0x00000;
	tiles[x].pattern[16] = 0x00000;
	tiles[x].pattern[17] = 0x00000;
	tiles[x].pattern[18] = 0x00000;
	tiles[x].pattern[19] = 0x00000;

	x = leftTile;
	tiles[x].colors[0]   = 0x0d0d0d;//fill color  : light black
	tiles[x].colors[1]   = 0xff0000;//detail color: red
	tiles[x].pattern[0]  = 0x00600;
	tiles[x].pattern[1]  = 0x00F00;
	tiles[x].pattern[2]  = 0x01F80;
	tiles[x].pattern[3]  = 0x03FC0;
	tiles[x].pattern[4]  = 0x07FE0;
	tiles[x].pattern[5]  = 0x00F00;
	tiles[x].pattern[6]  = 0x00F00;
	tiles[x].pattern[7]  = 0x00F00;
	tiles[x].pattern[8]  = 0x00F00;
	tiles[x].pattern[9]  = 0x00F00;
	tiles[x].pattern[10] = 0x00F00;
	tiles[x].pattern[11] = 0x00F00;
	tiles[x].pattern[12] = 0x00F00;
	tiles[x].pattern[13] = 0x00F00;
	tiles[x].pattern[14] = 0x00F00;
	tiles[x].pattern[15] = 0x00F00;
	tiles[x].pattern[16] = 0x00F00;
	tiles[x].pattern[17] = 0x00F00;
	tiles[x].pattern[18] = 0x00F00;
	tiles[x].pattern[19] = 0x00F00;

	x = rightTile;
	tiles[x].colors[0]   = 0x0d0d0d;//fill color  : light black
	tiles[x].colors[1]   = 0x006600;//detail color: green
	tiles[x].pattern[0]  = 0x00F00;
	tiles[x].pattern[1]  = 0x00F00;
	tiles[x].pattern[2]  = 0x00F00;
	tiles[x].pattern[3]  = 0x00F00;
	tiles[x].pattern[4]  = 0x00F00;
	tiles[x].pattern[5]  = 0x00F00;
	tiles[x].pattern[6]  = 0x00F00;
	tiles[x].pattern[7]  = 0x00F00;
	tiles[x].pattern[8]  = 0x00F00;
	tiles[x].pattern[9]  = 0x00F00;
	tiles[x].pattern[10] = 0x00F00;
	tiles[x].pattern[11] = 0x00F00;
	tiles[x].pattern[12] = 0x00F00;
	tiles[x].pattern[13] = 0x00F00;
	tiles[x].pattern[14] = 0x00F00;
	tiles[x].pattern[15] = 0x07FE0;
	tiles[x].pattern[16] = 0x03FC0;
	tiles[x].pattern[17] = 0x01F80;
	tiles[x].pattern[18] = 0x00F00;
	tiles[x].pattern[19] = 0x00600;

	x = characterTile;
	tiles[x].colors[0]   = 0x0d0d0d;//fill color  : light black
	tiles[x].colors[1]   = 0xffffff;//detail color: white
	tiles[x].pattern[0]  = 0x00000;
	tiles[x].pattern[1]  = 0x00000;
	tiles[x].pattern[2]  = 0x00000;
	tiles[x].pattern[3]  = 0x00000;
	tiles[x].pattern[4]  = 0x00000;
	tiles[x].pattern[5]  = 0x07000;
	tiles[x].pattern[6]  = 0x08000;
	tiles[x].pattern[7]  = 0x103FC;
	tiles[x].pattern[8]  = 0x20000;
	tiles[x].pattern[9]  = 0x20000;
	tiles[x].pattern[10] = 0x20000;
	tiles[x].pattern[11] = 0x20000;
	tiles[x].pattern[12] = 0x103FC;
	tiles[x].pattern[13] = 0x08000;
	tiles[x].pattern[14] = 0x07000;
	tiles[x].pattern[15] = 0x00000;
	tiles[x].pattern[16] = 0x00000;
	tiles[x].pattern[17] = 0x00000;
	tiles[x].pattern[18] = 0x00000;
	tiles[x].pattern[19] = 0x00000;
}//init_tiles
