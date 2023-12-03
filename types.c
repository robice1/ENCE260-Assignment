/* 
 * ENCE260 Assignment: Battleships
 * types.c: Source code for handling types useful for game infrastructure.
 * Authors: Robert Ivill & Findlay Royds
 * Date: 8/10/2023
 * */

#include "types.h"

Vector_t initVector (int8_t x, int8_t y)
{
    Vector_t vector = {x, y};
    return vector;
}

Vector_t addVectors (Vector_t vector1, Vector_t vector2)
{
    Vector_t vector = {vector1.x + vector2.x, vector1.y + vector2.y};
    return vector;
}

void clampVector (Vector_t *vector, Vector_t lowerBound, Vector_t upperBound)
{
    vector->x = max (min (vector->x, upperBound.x), lowerBound.x);
    vector->y = max (min (vector->y, upperBound.y), lowerBound.y);
}

void keepShipInBounds (Ship_t *ship)
{
    if (ship->isHorizontal) {
        clampVector (&ship->position, initVector (0, 0), initVector (NUM_COLS - ship->length, NUM_ROWS - 1));
    } else {
        clampVector (&ship->position, initVector (0, 0), initVector (NUM_COLS - 1, NUM_ROWS - ship->length));
    }
}

void getShipPoints (Vector_t *buffer, Ship_t ship)
{
    for (uint8_t i = 0; i < ship.length; i++) {
        // Add offset to ship position depending on if the ship is horizontal and i
        Vector_t newVector = addVectors (ship.position, initVector (ship.isHorizontal ? i : 0, !ship.isHorizontal ? i : 0));
        buffer[i] = newVector;
    }
}

void initialiseGameData (SharedData_t *gameData)
{

    gameData->gameState = PLACING;
    gameData->cursor = initVector (0, 0);

    gameData->navswitchInput = initVector (0, 0);
    gameData->navswitchPressed = false;
    gameData->navswitchIsCentred = false;
    gameData->buttonPressed = false;

    gameData->logicTick = 0;

    Ship_t newShip = {ship_lengths[0], initVector (0, 0), true};
    gameData->placingShip = newShip;
    gameData->placingShipId = 0;

    setBoardZero (&gameData->playerShips);
    setBoardZero (&gameData->hits);
    setBoardZero (&gameData->misses);
    
    gameData->lostGame = 0;
    gameData->numHits = 0;
    gameData->numOtherHits = 0;

    gameData->playerRole = UNDECIDED;

    gameData->previousShot = NONE;
}

void setBoardZero (BoardBitMask_t *board)
{
    for (uint8_t y = 0; y < 5; y++) {
        for (uint8_t x = 0; x < 7; x++) {
            setBoardPoint (board, initVector (x, y), 0);
        }
    }
}

bool getBoardPoint (BoardBitMask_t *board, Vector_t point)
{
    return ((*board)[point.y] >> point.x) & 0x1;
}

void setBoardPoint (BoardBitMask_t *board, Vector_t point, bool value)
{
    if (value)
        (*board)[point.y] |= 1 << point.x;
    else
        (*board)[point.y] &= 0 << point.x;
}