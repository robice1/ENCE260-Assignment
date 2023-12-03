/* 
 * ENCE260 Assignment: Battleships
 * render.c: Source code for rendering the display throughout playtime.
 * Authors: Robert Ivill & Findlay Royds
 * Date: 8/10/2023
 * */

#include "render.h"

static GameState_t previousGameState;

/*Converts a Vector_t into a tinygl_point
* Declared in render for lower coupling (tinygl is not included in any other modules)*/
static tinygl_point_t vectorToPoint (Vector_t vector)
{
    return tinygl_point (vector.y, vector.x * -1 + NUM_COLS - 1);
}

/*Render where the player placed their ships*/
static void renderShip (Ship_t ship)
{
    // code to test the get ship points function
    Vector_t buffer[ship.length];
    getShipPoints (buffer, ship);
    for (uint8_t i = 0; i < ship.length; i++) {
        tinygl_draw_point (vectorToPoint (buffer[i]), true);
    }
}

/*Render whether you are a winner or a loser*/
static void renderWinOrLose (bool isWinner)
{
    if (isWinner) {
        tinygl_text ("YOU WIN!");
    } else {
        tinygl_text ("YOU LOSE...");
    }
}

/* Renders the bitmask of a board*/
static void renderBoardBitMask (BoardBitMask_t *board)
{
    for (uint8_t y = 0; y < NUM_ROWS; y++) {
        for (uint8_t x = 0; x < NUM_COLS; x++) {
            Vector_t point = initVector (x, y);
            if (getBoardPoint (board, point) == true)
                tinygl_draw_point (vectorToPoint (point), true);
        }
    }
}

/*Render placing ships on your board*/
static void renderPlacing (SharedData_t *gameData)
{
    tinygl_clear ();
    renderBoardBitMask (&gameData->playerShips);

    // Make ship being placed flash unless it's moving. TODO update detecting when the ship is moving
    if (gameData->logicTick % 2 || !gameData->navswitchIsCentred)
        renderShip (gameData->placingShip);

    //Set text if game state has changed to waiting
}

/*Render firing at the other player*/
static void renderFiring (SharedData_t *gameData)
{
    tinygl_clear ();

    renderBoardBitMask (&gameData->hits);
    
    if ((gameData->logicTick % MISS_FLASH_TICKS) >= MISS_FLASH_OFF) {
        renderBoardBitMask (&gameData->misses);
    }

    tinygl_draw_point (vectorToPoint (gameData->cursor), gameData->logicTick % CURSOR_FLASH_TICKS == 0 || !gameData->navswitchIsCentred);
}

/*Render waiting for turn*/
static void renderWaiting (SharedData_t *gameData)
{
    // Set text if game state has changed to waiting
    switch (gameData->previousShot) {
        case NONE: {
            // First round where no shot was taken
            tinygl_clear ();
            tinygl_text ("WAITING");
            break;
        }
        case UNKNOWN: {
            // Do nothing, either waiting for status or status has been cleared
            break;
        }
        case HIT: {
            tinygl_clear ();
            tinygl_text ("HIT!");
            break;
        }
        case MISS: {
            tinygl_clear ();
            tinygl_text ("MISS");
            break;
        }
    }
    gameData->previousShot = UNKNOWN;
}

/*Render Game Over state*/
static void renderGameOver (SharedData_t *gameData)
{
    if (gameData->gameState != previousGameState) {
        tinygl_clear ();
        renderWinOrLose (!gameData->lostGame);
    }
}

/*Render ready to play game state (waiting for other player to place ships)*/
static void renderReadyToPlay (SharedData_t *gameData)
{
    if (gameData->gameState != previousGameState) {
        tinygl_clear ();
        tinygl_text ("READY");
    }
}

void renderTask (void *data)
{
    SharedData_t *gameData = data;

    switch (gameData->gameState) {
        case PLACING: {
            renderPlacing (gameData);
            break;
        }
        case FIRING: {
            renderFiring (gameData);
            break;
        }
        case WAITING: {
            renderWaiting (gameData);
            break;
        }
        case READY_TO_PLAY: {
            renderReadyToPlay (gameData);
            break;
        }
        case GAMEOVER: {
            renderGameOver (gameData);
            break;
        }
    }

    // Used to track the game state from the previous iteration. Used to detect when the state of the game has just changed
    previousGameState = gameData->gameState;

    tinygl_update ();
}

void renderInit (void *data, int render_rate)
{
    tinygl_init (render_rate);
    tinygl_font_set (&font3x5_1);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
    tinygl_text_dir_set (TINYGL_TEXT_DIR_ROTATE);
    tinygl_text_speed_set (TEXT_SCROLL_SPEED);

    previousGameState = ((SharedData_t*) data)->gameState;
}