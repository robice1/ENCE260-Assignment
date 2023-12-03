/* 
 * ENCE260 Assignment: Battleships
 * ir_handler.c: Source code for handling IR communications throughout playtime.
 * Authors: Robert Ivill & Findlay Royds
 * Date: 8/10/2023
 * */


#include "ir_handler.h"

// The game state of the previous ir task. Used to track when the state has changed
static GameState_t previousGameState;

void irInit (void)
{
    ir_uart_init ();
}

/*Clear any unexpected characters which have not been read*/
static void irClear (void)
{
    while (ir_uart_read_ready_p ())
        ir_uart_getc ();
}

/* Sends YOUR_TURN to other player*/
static void irSendYourTurn (void)
{
    while (!ir_uart_write_ready_p ()) {
            continue;
        }
    ir_uart_putc (YOUR_TURN);
}

/* Sends a Vector of the shot position*/
static void irSendShot (Vector_t shot)
{
    char send = (((shot.x & VECTOR_X_MASK) << VECTER_X_SHIFT) | (shot.y & VECTOR_Y_MASK));

    while (!ir_uart_write_ready_p ()) {
        continue;
    }

    ir_uart_putc (send);
}

/* Unpacks a shot from IR into a vector*/
static Vector_t irReceiveShot (char recv)
{
    Vector_t result;
    result.x = (uint8_t) ((recv >> VECTER_X_SHIFT) & VECTOR_X_MASK);
    result.y = (uint8_t) (recv & VECTOR_Y_MASK);
    return result;
}

/* Process ir communication for the waiting state of the game
 * Responsible for sending and recieving information about shots fired*/
static void irHandleWaiting (SharedData_t *gameData)
{
    // If previous game state was firing, the player has taken a shot
    // Alert the other player about the shot location to get whether it was a hit or a miss
    if (previousGameState == FIRING) {
        irClear ();
        irSendYourTurn ();

        irSendShot (gameData->cursor);

        // Get whether the shot was a hit or miss
        while (!ir_uart_read_ready_p ()) {
            continue;
        }
        char hitResult = ir_uart_getc ();

        if (hitResult == RESULT_HIT) { // Shot was a hit
            setBoardPoint (&gameData->hits, gameData->cursor, true);
            gameData->previousShot = HIT;
            
            // Check whether the player won the game
            gameData->numHits++;
            if (gameData->numHits >= NUM_SHIP_POINTS) {
                gameData->gameState = GAMEOVER;
            }
        } else { // Shot was a miss
            setBoardPoint (&gameData->misses, gameData->cursor, true);
            gameData->previousShot = MISS;
        }
    }

    // Waiting to hear from other player that they've fired a shot
    // Switch to firing once they have
    if (ir_uart_read_ready_p ()) {
        char readChar = ir_uart_getc ();
        if (readChar == YOUR_TURN) {
            // Wait for vector of shot
            while (!ir_uart_read_ready_p ()) {
                continue;
            }
            char rec = ir_uart_getc ();
            Vector_t shotPosition = irReceiveShot (rec);
            
            // Check whether the other player's shot was a hit or miss and let them know
            bool isHit = getBoardPoint (&gameData->playerShips, shotPosition);

            while (!ir_uart_write_ready_p ()) {
                continue;
            }
            if (isHit) {
                ir_uart_putc (RESULT_HIT);
                
                // Check if other player won the game with this shot
                gameData->numOtherHits++;
                if (gameData->numOtherHits >= NUM_SHIP_POINTS) {
                    gameData->lostGame = true;
                    gameData->gameState = GAMEOVER;
                    return;
                }
            } else {
                ir_uart_putc (RESULT_MISS);
            }

            gameData->gameState = FIRING;
        }
    }
}

/* Handles the communications for the ready to play state, which is after the player's ships have been placed
 * Deals with working out which player goes first*/
static void irHandleReadyToPlay (SharedData_t *gameData)
{
    // When other player has already placed their ships, go to the waiting state and let them know you've placed your ships
    if (gameData->playerRole == UNDECIDED) {
        if (ir_uart_read_ready_p ()) {
            if (ir_uart_getc () == READY) {
                gameData->playerRole = PLAYER_TWO;
                gameData->gameState = WAITING;

                while (!ir_uart_write_ready_p ()) {
                    continue;
                }
                ir_uart_putc (YOUR_TURN);
            }
        }
    }
    
    // Other player has not placed their ships, meaning you are player one.
    // Let the other player know you've placed your ships
    if (gameData->playerRole == UNDECIDED) {
        gameData->playerRole = PLAYER_ONE;
        
        while (!ir_uart_write_ready_p ()) {
            continue;
        }
        ir_uart_putc (READY);
    }

    // Waiting to hear from the other player that they've placed their ships
    // Switch to firing once they have
    if (gameData->playerRole == PLAYER_ONE) {
        if (ir_uart_read_ready_p ()) {
            if (ir_uart_getc () == YOUR_TURN) {
                gameData->gameState = FIRING;
            }
        }
    }
}

void irTask (void *data)
{
    SharedData_t *gameData = data;
    switch (gameData->gameState) {
        case FIRING: {
            // No communication needs to happen in the firing state
            break;
        }
        case WAITING: {
            irHandleWaiting (gameData);
            break;
        }
        case GAMEOVER: {
            // No communication needs to happen in the game over state
            break;
        }
        case PLACING: {
            // No communication needs to happen in the placing state
            break;
        }
        case READY_TO_PLAY: {
            irHandleReadyToPlay (gameData);
            break;
        }
        // There should be no additional characters to be read, discard any unused ones
        irClear ();
    }

    previousGameState = gameData->gameState;
}