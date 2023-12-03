/* 
 * ENCE260 Assignment: Battleships
 * game.h: Header file for game.c.
 * Authors: Robert Ivill & Findlay Royds
 * Date: 8/10/2023
 * */

#ifndef GAME_H
#define GAME_H

/* Library Modules */
#include "system.h"
#include "task.h"

/* Game Modules*/
#include "types.h"
#include "render.h"
#include "logic.h"
#include "ir_handler.h"
#include "input.h"

/* Define polling rates in Hz.  */
#define RENDER_TASK_RATE 500
#define IR_TASK_RATE 100
#define LOGIC_TASK_RATE 8
#define INPUT_TASK_RATE 100

#endif