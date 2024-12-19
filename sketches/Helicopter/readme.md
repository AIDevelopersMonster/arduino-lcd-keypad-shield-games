# Helicopter Game for Arduino LCD

This project is an adaptation of the classic "Helicopter" game for Arduino, using an LCD display and a speaker for sound effects. The original game was created by [Kevin Loney](https://makezine.com/2013/03/11/arduino-helicopter-game-2/) and is available under the [Creative Commons Attribution-ShareAlike 3.0 Unported License](http://creativecommons.org/licenses/by-sa/3.0/).

In this version, we made several improvements, including:

- **Character Animation**: Custom animations for the helicopter's tail.
- **Button Control**: Button input is read using a resistive voltage divider, making it easier to connect buttons.
- **Sound Effects**: Musical and sound effects have been added, including sounds when the game starts, when the player wins, and when a collision occurs.
- **High Score Saving**: High scores are saved to EEPROM and displayed on the screen.
- **LCD Customization**: Custom characters are used for sprites, walls, and explosions.

## Features

- **Smooth Animation**: The helicopter tail animates with custom sprites on the LCD screen.
- **Dynamic Difficulty**: The game's difficulty increases over time as the obstacles speed up.
- **Sound Effects**: The game features a dynamic soundtrack with tones for various events, including collision detection.
- **Highscore Storage**: High scores are stored in EEPROM and persist after powering down the Arduino.

## Video

Here’s a video demonstration of the game:

[![Watch the video](https://img.youtube.com/vi/hVHkjJM4h28/0.jpg)](https://www.youtube.com/shorts/hVHkjJM4h28)

Click the image above or follow this [link](https://www.youtube.com/shorts/hVHkjJM4h28) to watch the gameplay in action.

## How to Run

1. **Hardware Requirements**:
   - Arduino board (Uno, Nano, etc.)
   - LCD 16x2 or 20x4 display (with I2C or direct connection)
   - Push button or joystick for control
   - Piezo buzzer or speaker for sound

2. **Software Setup**:
   - Download and install the Arduino IDE.
   - Clone or download this repository to your local machine.
   - Open the `.ino` file in the Arduino IDE.
   - Connect your Arduino to your computer and upload the code.
   - Once uploaded, you can play the game on the connected LCD screen.

## License

This project is licensed under the [Creative Commons Attribution-ShareAlike 3.0 Unported License](http://creativecommons.org/licenses/by-sa/3.0/). You are free to use, modify, and distribute this code, provided you credit the original author and share your modifications under the same license.

## Links

- [Original Game by Kevin Loney](https://makezine.com/2013/03/11/arduino-helicopter-game-2/)
- [YouTube Channel](https://www.youtube.com/channel/UCXXXXXXXXXXXXXXX)
- [Website](http://www.kontakts.ru)

## Acknowledgements

- **Original Author**: [Kevin Loney](https://makezine.com/2013/03/11/arduino-helicopter-game-2/)
- **Video**: [Watch the demo on YouTube](https://www.youtube.com/shorts/hVHkjJM4h28)

Feel free to contribute, fork the project, or share your own adaptations and improvements!
