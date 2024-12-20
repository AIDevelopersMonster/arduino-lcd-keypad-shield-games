# Pacman Game on Arduino Uno & LCDKeypadShield

## Overview
This is a simple yet engaging implementation of the classic **Pacman Game**, designed to run on an **Arduino Uno** with an **LCD Keypad Shield**. The game is fully interactive, featuring movement controls, collectible dots, and a pursuing ghost.

### How It Works:
- **Objective**: Guide Pacman to eat all the dots on the screen while avoiding the ghost.
- **Game Controls**: Use the LCD Keypad Shield buttons to navigate Pacman:
  - **Left**: Move left
  - **Right**: Move right
  - **Up**: Move up
  - **Down**: Move down
- **Progression**:
  - If you eat all the dots, you advance to the next level with increased difficulty.
  - If the ghost catches Pacman, the game is over.

---

## Features
- **LCD Display**:
  - Displays the game grid.
  - Shows Pacman, the ghost, and dots.
- **Sound Effects**:
  - A buzzer provides feedback for eating dots, ghost encounters, and other events.
- **Dynamic Levels**:
  - Each level gets progressively harder as the ghost speeds up.
- **Custom Characters**:
  - Pacman, the ghost, and dots are rendered using custom LCD characters.

---

## Hardware Requirements
To play the game, you need the following components:
- **Arduino Uno** (or compatible board).
- **LCD Keypad Shield** for display and controls.
- **Buzzer** for sound effects.
- **USB Cable** for power and uploading the sketch.

---

## Installation
1. Clone the repository:
   ```bash
   git clone https://github.com/AIDevelopersMonster/arduino-lcd-keypad-shield-games.git
   ```
2. Navigate to the Pacman sketch:
   ```bash
   cd arduino-lcd-keypad-shield-games/sketches/Pacman
   ```
3. Open the `Pacman.ino` file in the Arduino IDE.
4. Connect your Arduino Uno to your computer using a USB cable.
5. Select the appropriate **Board** and **Port** in the Arduino IDE.
6. Upload the sketch to your Arduino Uno.

---

## How to Play
1. Power the Arduino and LCD Keypad Shield.
2. Use the buttons to control Pacman’s movement:
   - **Left, Right, Up, Down**.
3. Avoid the ghost and eat all the dots to progress to the next level.
4. If the ghost catches Pacman, the game ends.

---

## Customization
Feel free to modify and enhance the game! Here are a few ideas:
- Add new levels with different layouts or challenges.
- Improve the ghost’s AI to make it more strategic.
- Add a scoring system with a leaderboard.
- Implement additional sound effects for specific game events.
- Modify the display to include a mini-map or additional player information.

---

## Credits
- Original Sketch: **Jean Malha**
- Modifications and Sound Effects: **[AIDevelopersMonster Team](https://github.com/AIDevelopersMonster)**

---

## Video Demo
Check out the gameplay demonstration on YouTube:
👉 [Watch the Pacman Game in Action](https://youtube.com/shorts/Z0TSHcleV8Y)

---

## Contributing
Contributions are welcome! If you have ideas for improving the game or find bugs, feel free to:
1. Fork the repository.
2. Make your changes.
3. Submit a pull request.

---

## License
This project is licensed under the MIT License. See the [LICENSE](../LICENSE) file for details.

