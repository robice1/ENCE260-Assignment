/* 
 * ENCE260 Assignment: Battleships
 * logic.c: Source code for handling game logic throughout playtime.
 * Authors: Robert Ivill & Findlay Royds
 * Date: 8/10/2023
 * */

#include "logic.h"

static bool previousNavswitchPressed;
static bool previousButtonPressed; 

/* Return true if the vector is within the bounds of the screen*/
static bool isValidVector (Vector_t vector)
{
    if (vector.x < 0 || vector.x >= NUM_COLS)
        return false;
    if (vector.y < 0 || vector.y >= NUM_ROWS)
        return false;
    return true;
}

/* Move the player's cursor with the navswitch input*/
static void moveCursor (SharedData_t *gameData)
{
    Vector_t newPosition = addVectors (gameData->cursor, gameData->navswitchInput);
    if (isValidVector (newPosition))
        gameData->cursor = addVectors (gameData->cursor, gameData->navswitchInput);
}

/* Make sure a move is valid on the board*/
static bool isValidMove (BoardBitMask_t *board, Ship_t ship)
{
    Vector_t shipPoints[ship.length];
    getShipPoints (shipPoints, ship);
    for (uint8_t i = 0; i < ship.length; i++)
        if (getBoardPoint (board, shipPoints[i]))
            return false;
    return true;
}

/* Places a ship on the player's board. Checks whether it is a valid placement before placing*/
static void placeShip (SharedData_t *gameData, Ship_t ship)
{
    if (!isValidMove (&gameData->playerShips, ship))
        return;

    // Update the player's board bit mask to include the ship being placed
    Vector_t shipPoints[ship.length];
    getShipPoints (shipPoints, ship);
    for (uint8_t i = 0; i < ship.length; i++)
        setBoardPoint (&gameData->playerShips, shipPoints[i], true);

    // Create the next ship for the player to place
    gameData->placingShipId++;
    if (gameData->placingShipId >= NUM_SHIPS) {
        gameData->gameState = READY_TO_PLAY;
        return;
    }

    Ship_t newShip = {ship_lengths[gameData->placingShipId], initVector (0, 0), true};
    gameData->placingShip = newShip;
}

/* Allows player to move and rotate a ship, and then place the ship on the board*/
static void processPlacing (SharedData_t *gameData)
{
    // Rotate the ship
    if (gameData->navswitchPressed && !previousNavswitchPressed)
        gameData->placingShip.isHorizontal = !gameData->placingShip.isHorizontal;

    // Move the ship
    gameData->placingShip.position = addVectors (gameData->placingShip.position, gameData->navswitchInput);
    keepShipInBounds (&gameData->placingShip);
    
    // Place the ship
    if (gameData->buttonPressed && !previousButtonPressed)
        placeShip (gameData, gameData->placingShip);
}

/* Process the logic for the firing state. Move the player's cursor and switch to waiting state when a shot is fired*/
static void processFiring (SharedData_t *gameData)
{
    moveCursor (gameData);

    if (gameData->buttonPressed) {
        bool validShot = !(getBoardPoint (&gameData->hits, gameData->cursor) || getBoardPoint (&gameData->misses, gameData->cursor));
        if (validShot) {
            gameData->gameState = WAITING;
            gameData->previousShot = UNKNOWN;
        }
    }
}

void logicInit (SharedData_t *gameData)
{
    initialiseGameData (gameData);
}

void logicTask (void *data)
{
    SharedData_t *gameData = data;

    switch (gameData->gameState) {
        case PLACING: {
            processPlacing (gameData);
            break;
        }
        case READY_TO_PLAY: {
            // No logic is needed for the ready to play state
            break;
        }
        case FIRING: {
            processFiring (gameData);
            break;
        }
        case WAITING: {
            // No logic is needed for the waiting state
            break;
        }
        case GAMEOVER: {
            // No logic is needed for the gameover state
            break;
        }
    }

    // Keep track of the current state of navswitch input for next time the task is run
    previousButtonPressed = gameData->buttonPressed;
    previousNavswitchPressed = gameData->navswitchPressed;
    gameData->logicTick++;
}