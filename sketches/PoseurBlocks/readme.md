Tetris Game for Arduino with LCD Keypad Shield
This project is a Tetris game implementation for Arduino using an LCD screen and keypad shield. It features a tetromino-based game where the player can rotate and move blocks to clear rows. The game uses custom characters on the LCD to represent tetromino shapes and handles user input through the keypad shield.

Features:
Tetromino Blocks: The game includes 7 different tetromino types ('I', 'J', 'L', 'O', 'S', 'T', and 'Z') that can be moved, rotated, and dropped.
LCD Display: Uses the Arduino LCD to show the game grid with custom characters.
Keypad Control: Move blocks left, right, down, or rotate using the keypad buttons.
Game Logic: Includes collision detection, clearing of full rows, and new tetromino generation.
Setup:
Hardware Requirements:

Arduino Uno or compatible board
LCD Keypad Shield (16x2 LCD)
Buzzer (optional for sound effects)
A speaker or buzzer connected to pin 3 for sound output.
Connections:

LCD Keypad Shield is plugged into the Arduino (no extra wiring needed if using a standard LCD shield).
Pin 3 is used for sound output.
Software Requirements:

Arduino IDE
LiquidCrystal library (for LCD control)
Random number generator (for tetromino randomness)
Usage:
Keypad Mapping:
LEFT: Move tetromino left
RIGHT: Move tetromino right
DOWN: Move tetromino down
ROTATE: Rotate tetromino
Gameplay:
The game starts with a random tetromino appearing at the top of the grid.
Players can control the tetromino using the keypad.
The objective is to fill horizontal rows with blocks, which will be cleared when fully completed.
Sound:
A simple sound effect plays when a tetromino moves or rotates.
Game Flow:
Start: The game initializes the screen and randomizes the starting tetromino.
Move: Players can move tetrominoes left, right, down, or rotate them.
Collision: The game checks for collisions with the walls or other blocks.
Row Clearing: Full rows are cleared, and new rows are added at the top.
Game Over: If a new tetromino cannot be placed at the top, the game ends.
Code Explanation:
Key Functions:
is_empty(): Checks if a position on the grid is empty (used to check for collisions).
move(): Moves the tetromino based on input (left, right, down, rotate).
draw_tetromino(): Draws the tetromino on the game grid.
create_new_char(): Creates custom LCD characters for each block of a tetromino.
find_full_rows(): Finds and returns the index of the first full row to be cleared.
print_to_lcd(): Updates the LCD to reflect the current state of the game grid.
add_new_tetromino(): Adds a new random tetromino to the game.
Control Flow:
setup(): Initializes game state, LCD, and the first tetromino.
loop(): Handles game progression (moves tetrominos, checks for full rows, etc.).
License:
sql
Копировать код
Copyright (c) 2015, Alex Zen <me@alexzen.net>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
    3. Neither the name of poseur-blocks nor the names of its
       contributors may be used to endorse or promote products derived from
       this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ''AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
How to Contribute:
Feel free to submit pull requests or open issues if you find bugs or want to add new features! If you want to improve the game or add more functionality, just fork the repository and contribute.
