/* 
 * ENCE260 Assignment: Battleships
 * ir_handler.h: Header file for ir_handler.c.
 * Authors: Robert Ivill & Findlay Royds
 * Date: 8/10/2023
 * */


#ifndef IR_HANDLER_H
#define IR_HANDLER_H

#include "ir_uart.h"
#include "types.h"

/* Data to be sent between UCFKs*/
#define YOUR_TURN 1
#define READY 3
#define PLAYERONE 7
#define RESULT_HIT 15
#define RESULT_MISS 31

/* Values for encoding and decoding a vector into an 8-bit char*/
#define VECTER_X_SHIFT 3 // How many bits the x value of the vector should be left shifted when encoded
#define VECTOR_X_MASK 0b1111 // A bitmask for the x value of the vector, maximum bits
#define VECTOR_Y_MASK 0b111 // A bitmask for the y value of the vector, maximum bits

/* Initialises the IR task*/
void irInit (void);

/* Handles tasks for the IR*/
void irTask (void *data);

#endif //IR_HANDLER_H