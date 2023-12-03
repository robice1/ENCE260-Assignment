/* 
 * ENCE260 Assignment: Battleships
 * logic.h: Header file for logic.c.
 * Authors: Robert Ivill & Findlay Royds
 * Date: 8/10/2023
 * */

#ifndef LOGIC_H
#define LOGIC_H

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "types.h"

/*Initialises the logic task. Initialises the shared game data*/
void logicInit (SharedData_t *gameData);

/*The task which calculates all of the game logic, such as game states, firing shots, etc.*/
void logicTask (void *data);

#endif //LOGIC_H