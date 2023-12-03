/* 
 * ENCE260 Assignment: Battleships
 * input.c: Source code for handling button/navswitch inputs throughout playtime.
 * Authors: Robert Ivill & Findlay Royds
 * Date: 8/10/2023
 * */

#include "input.h"

void inputInit () {
    navswitch_init ();
    button_init ();
}

void inputTask (void *data) {
    SharedData_t *gameData = data;

    // navswitch
    navswitch_update ();

    gameData->navswitchInput.x = 0;
    gameData->navswitchInput.y = 0;

    if (navswitch_down_p (NAVSWITCH_WEST))
        gameData->navswitchInput.y--;
    else if (navswitch_down_p (NAVSWITCH_EAST))
        gameData->navswitchInput.y++;
    
    if (navswitch_down_p (NAVSWITCH_SOUTH))
        gameData->navswitchInput.x--;
    else if (navswitch_down_p (NAVSWITCH_NORTH))
        gameData->navswitchInput.x++;
    
    gameData->navswitchIsCentred = (gameData->navswitchInput.x == 0 && gameData->navswitchInput.y == 0);
    
    gameData->navswitchPressed = navswitch_down_p (NAVSWITCH_PUSH);

    // button
    button_update ();

    gameData->buttonPressed = button_down_p (0);
}