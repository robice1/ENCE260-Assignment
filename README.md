# BATTLESHIPS FOR THE UCFK4


This repository stores the Battleships game for the UCFK4

**Authors**: Robert Ivill and Findlay Royds

**Date**: October 2023

## Setting up the gane

1. Download AVR tools from the terminal:
```
$ sudo apt-get install git gcc-avr binutils-avr avr-libc dfu-programmer
```


2. Clone the [UCFK4 Repository](https://eng-git.canterbury.ac.nz/mph/ucfk4):
```
$ git clone https://eng-git.canterbury.ac.nz/mph/ucfk4.git
```

3. Clone the game application repository into the UCFK4 apps directory:
```
$ cd ucfk4/apps
$ git clone https://eng-git.canterbury.ac.nz/ence260-2023/group_335.git
$ cd group_335
```

4. Build the game using `make`:
```
$ make 
# compiles source code and builds object files
$ make program 
# runs 'make' and loads program into UCFK4. Run this to begin playing
$ make clean 
# removes old object files 
```

## Gameplay

1. Place ships: 

Use the navigation switch to move the current ship (flashing pixels), press navigation switch to rotate your ship and finally press the button to place your ships.

2. Start the game:

Make sure the UCFKs are within IR range before both players have placed their ships to begin the game.

3. Fire missiles:

Take turns shooting missiles at the other player's ships. Successful strikes show up as a non-flashing pixel and misses show up as slowly flashing pixels. Use the navigation switch to move the cursor (fast flashing pixel) around the board and use the button to fire. You will be notified if you've hit or missed their battleships and the other players turn will begin.

4. End of game:

If a player has successfully sunk all of the other players ships, this will trigger the game to end and you will be notified as a winner or loser. Hold the button and press the reset button to play again.

Thanks for playing our game!