/* 
 * ENCE260 Assignment: Battleships
 * game.c: Source code of main function for initialisation and task scheduling.
 * Authors: Robert Ivill & Findlay Royds
 * Date: 8/10/2023
 * */
#include "game.h"


/*Main program, initialises tasks and runs task scheduler*/
int main (void)
{
    // Creates shared game data to give each task access to
    SharedData_t gameData;

    // Defines tasks and their rates
    task_t tasks[] =
    {
        {.func = renderTask, .period = TASK_RATE / RENDER_TASK_RATE, .data = &gameData},
        {.func = logicTask, .period = TASK_RATE / LOGIC_TASK_RATE, .data = &gameData},
        {.func = inputTask, .period = TASK_RATE / INPUT_TASK_RATE, .data = &gameData},
        {.func = irTask, .period = TASK_RATE / IR_TASK_RATE, .data = &gameData},
    };

    system_init();

    // Initialise the tasks
    logicInit (&gameData);
    renderInit (&gameData, RENDER_TASK_RATE);
    inputInit ();
    irInit ();
    
    // Run the task scheduler to start the game
    task_schedule (tasks, ARRAY_SIZE (tasks));
    return 0;
}
