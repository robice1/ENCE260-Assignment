/* 
 * ENCE260 Assignment: Battleships
 * types.h: Header file for types.c. Defines many types and constants used by other modules
 * Authors: Robert Ivill & Findlay Royds
 * Date: 8/10/2023
 * */

#ifndef TYPES_H
#define TYPES_H

#include "stdint.h"
#include "stdbool.h"

/* Number of collumns and rows on the led matrix*/
#define NUM_ROWS 5
#define NUM_COLS 7

#define TEXT_SCROLL_SPEED 25 //The speed at which text scrolls across the sceen

// Macros for min and max (similar to python's min and max)
#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

/* Custom vector type. Used instead of tinygl_point to reduce coupling*/
typedef struct {
    int8_t x;
    int8_t y;
} Vector_t;

/* Custom datatype for storing information about a ship*/
typedef struct {
    uint8_t length; // The number of points the ship occupies
    Vector_t position; // Highest xcoord and ycoord of ship
    bool isHorizontal; // True if the ship is sideways, otherwise ship is vertical
} Ship_t;

/* The order and lengths to place ships on the board*/
static const uint8_t ship_lengths[4] = {4, 3, 3, 2};
#define NUM_SHIPS 4
#define NUM_SHIP_POINTS 12 // The total number of points the ships occupy. Sum of lengths of the ships

/* Current state of the game*/
typedef enum {
    PLACING, // Placing ships on your own board
    FIRING, // Firing a shot at the other player
    WAITING, // Waiting for the other player to fire their shot
    GAMEOVER, // Game has finished
    READY_TO_PLAY // Player has placed their ships, waiting to recieve other player's board
} GameState_t;

/* Bitmask for data on the board, such as ships, shots fired, etc.*/
typedef uint8_t BoardBitMask_t[5];

/* Which Player Role you currently have*/
typedef enum {
    UNDECIDED, // Have not communicated roles yet
    PLAYER_ONE, // First player to place their ships
    PLAYER_TWO // Second player to place their ships
} PlayerState_t;

/* Used to track the state of the previous shot*/
typedef enum {
    UNKNOWN, // For before the other board has replied with hit or miss. Also used by render.c after the hit status has been displayed.
    NONE, // For the first round where there was no previous shot
    HIT, // When the shot the player just took hit one of the other person's ships
    MISS // When the shot the player just took did not hit one of the other person's ships
} HitStatus_t;

/* Data shared between the logic, render, input, and ir tasks*/
typedef struct {
    GameState_t gameState; // What state the game is in, e.g., firing, game over
    Vector_t cursor; // What position the player's cursor is, used for firing a shot

    Vector_t navswitchInput; // Vector representing navswitch position, y = 1 represents up, x = -1 represents left
    bool navswitchPressed; // True if the navswitch is pressed down
    bool navswitchIsCentred; // True if the navswitch is in the centre / no input applied
    bool buttonPressed; // True if the push button by the display is pressed

    uint16_t logicTick; // Increments by one each time the logic task runs

    Ship_t placingShip; // The ship object which is currently being moved / placed on the board
    uint8_t placingShipId; // The id of the ship_lengths array which is currently being placed
    
    BoardBitMask_t playerShips; // Where the ships are on the player's board

    BoardBitMask_t misses; // Where the player has shot and missed
    BoardBitMask_t hits; // Where the player has shot and hit one of the other player's ships

    PlayerState_t playerRole; // Your player role (player 1 or player 2)
    
    bool lostGame; // Tracks whether the player won or lost, 0 by default
    uint8_t numHits; // tracks number of hits the player has
    uint8_t numOtherHits; // tracks number of hits the other player has

    HitStatus_t previousShot; // Tracks the status of the previous shot fired by the player e.g., hit, miss
} SharedData_t;

/* Create and return a new Vector_t from  an x and y value*/
Vector_t initVector (int8_t x, int8_t y);

/* Returns the result of adding two Vector_t's
 * Not passed as pointers as each Vector_t is only 2 bytes and does not get modified*/
Vector_t addVectors (Vector_t vector1, Vector_t vector2);

/* Clamps a given vector between two other vectors. e.g, (3, -7), (4, 5), (6, -5) would return (4, -5)*/
void clampVector (Vector_t *vector, Vector_t lowerBound, Vector_t upperBound);

/* Moves a ship so that it fits within the screen*/
void keepShipInBounds (Ship_t *ship);

/* Fill the provided buffer with Vector_t's of each point of the ship*/
void getShipPoints (Vector_t *buffer, Ship_t ship);

/* Sets all of the points on a board bit mask to 0*/
void setBoardZero (BoardBitMask_t *board);

/* Sets a point on the board bit mask to the specified value*/
void setBoardPoint (BoardBitMask_t *board, Vector_t point, bool value);

/* Returns either a 1 or a 0 depending on the value at the given point in a board bit mask*/
bool getBoardPoint (BoardBitMask_t *board, Vector_t point);

/* Initialises the given SharedData_t type with default values*/
void initialiseGameData (SharedData_t *gameData);

#endif //TYPES_H