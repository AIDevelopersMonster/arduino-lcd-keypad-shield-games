# DinosaurGame for Arduino LCD Keypad Shield

This is a simple implementation of the popular **Dinosaur Game** on an Arduino using the **LCD Keypad Shield**. The game features a dinosaur that jumps over obstacles, with the goal being to avoid collisions for as long as possible. This project was designed to run on an **Arduino Uno** or compatible board and uses a **16x2 LCD** display with a keypad shield for user input.

## Features

- **LCD Display**: The game is displayed on a 16x2 LCD screen.
- **Button Controls**: Use the buttons on the LCD Keypad Shield to control the dinosaur (UP to jump, DOWN to crouch).
- **Obstacles**: Random obstacles appear that the dinosaur needs to jump over or avoid.
- **Sound Effects**: Includes sound effects for different events like starting the game, jumping, collisions, and game over.

## Changes and Improvements

### 1. **Added Start Screen (Setup)**
   - A "DINOSAUR GAME" screen is displayed when the game starts, along with the prompt "PRESS ANY KEY" on the second line.
   - The game waits for a key press before starting. This ensures the user has time to see the start screen and get ready.

### 2. **Added Music**
   - **Startup Music**: A simple melody plays when the game starts, providing audio feedback to indicate the game is launching.
   - **Game Over Music**: A different melody plays when the game ends, adding an extra layer of feedback when the player loses.
   - **Button Press Sound**: A short sound is played when the user presses any button, providing tactile feedback during gameplay.
   - **Jump Sound**: A unique sound is played when the dinosaur jumps.
   - **Collision Sound**: Another sound plays when the dinosaur collides with an obstacle.

### 3. **Sound Integration**
   - All sound effects are generated using a passive buzzer connected to the Arduino, with the `tone()` function used to control the frequency and duration of the sounds.

## How to Play

1. When the game starts, you'll see the "DINOSAUR GAME" title and the "PRESS ANY KEY" message.
2. Press any button on the keypad to start the game.
3. The dinosaur will begin running, and you'll need to press the UP button to make it jump over obstacles.
4. The game ends when the dinosaur collides with an obstacle.

## Screenshot

![Dinosaur Game Screenshot](https://youtube.com/shorts/m07JnKcBQ5A)

## Video

Check out the gameplay in this YouTube video: [Dinosaur Game Video](https://youtube.com/shorts/m07JnKcBQ5A)

## Credits

- The original source code for this project was inspired by a pastebin code: [Original Source](https://pastebin.com/raw/h0Xnpmxc).
- Music and sound effects were added to enhance the gameplay experience.
- The game was developed and adapted by [Erbol Syzdyk](https://www.youtube.com/watch?v=pR_U7vryuJY).

Enjoy playing the Dinosaur Game on your Arduino!
