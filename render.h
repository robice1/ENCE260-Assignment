/* 
 * ENCE260 Assignment: Battleships
 * render.h: Header file for render.c.
 * Authors: Robert Ivill & Findlay Royds
 * Date: 8/10/2023
 * */


#ifndef RENDER_H
#define	RENDER_H

#include "system.h"
#include "tinygl.h"
#include "font.h"
#include "../fonts/font3x5_1.h"

#include "types.h"

#define SCROLL_SPEED 10

// Number of ticks between cursor flashes
#define CURSOR_FLASH_TICKS 2

#define MISS_FLASH_TICKS 6 // The number ticks between flashes for miss points
#define MISS_FLASH_OFF 3 // How much a miss point will be off when flashing. time off = MISS_FLASH_OFF / MISS_FLASH_TICKS

/*The render task which deals with displaying the current state of the game on the led matrix*/
void renderTask (void *data);

/*Initialise render*/
void renderInit (void *data, int render_rate);

#endif //RENDER_H