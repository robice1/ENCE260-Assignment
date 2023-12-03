/* 
 * ENCE260 Assignment: Battleships
 * input.h: Header file for input.c.
 * Authors: Robert Ivill & Findlay Royds
 * Date: 8/10/2023
 * */

#ifndef INPUT_H
#define INPUT_H

#include "navswitch.h"
#include "button.h"
#include "types.h"

/* Initialises the input task. initialises the button and navswitch*/
void inputInit (void);

/* Runs the input task. Reads the input of the navswitch and button and updates the shared game data*/
void inputTask (void *data);

#endif //INPUT_H