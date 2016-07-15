#ifndef __AUDIO_H_
#define __AUDIO_H_

/*******************************
 * Defines
 *******************************
 */		
#define WAVE_RES 128

/*******************************
 * Function Defintions
 *******************************
 */	
void initAudio(void);

/*******************************
 * Global Variables
 *******************************
 */			
extern const unsigned short sineParts[WAVE_RES];

#endif //__AUDIO_H_
