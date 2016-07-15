//for setting up and refreshing map

#ifndef __MAP_H_
#define __MAP_H_

/*******************************
 * Defines
 *******************************
 */																						
#define startX 0
#define startY 0
#define map_height 12
#define map_width  16		

/********************************
 * Type Definitions 
 ********************************
 */
typedef struct _tile
{
	int  colors  [2];//unsigned 32 bit integer, used to store color hexs
	int  pattern[20];//64 bit integer, used to store bit pattern for tiles
} Tile;	

typedef enum _tileIndex { unvisitedTile  = 0,
												  visitedTile    = 1,
												  magma          = 2,
												  ice            = 3,
												  breakableBlock = 4,
												  upTile         = 5,
												  downTile       = 6,
												  leftTile       = 7,
												  rightTile      = 8,
												  characterTile  = 9 } TileIndex;


/*******************************
 * Function Definitions
 *******************************
 */																																					 
void write_tile(Tile tile, int start_x, int start_y);                        
void init_tiles(void);
void refresh_map(void);	//probably also want to make local refresh		
void local_refresh(void);
void reset_map(void);												
											
/*******************************
 * Global Variables
 *******************************
 */																					
extern       int  playerX, playerY;													
extern       Tile tiles[10];													
extern const int  ref_map[map_height][map_width];
extern       int  map[map_height][map_width];

#endif //__MAP_H_

