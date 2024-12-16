// Автор изменений: AlexMalachevsky & ChatGPT
// 
//
// Внесенные изменения:
// 1. **Game Over**: Добавлен экран "Game Over", который появляется, когда мяч промахивается мимо ракетки. 
//    Игра предлагает начать заново, когда игрок нажимает кнопку.
//
// 2. **Обработка промаха**: В случае промаха мяч сбрасывается в начальную позицию, а игра переходит в режим "Game Over".
//    Это предотвращает продолжение игры после неудачного удара.
//
// 3. **Логика победы**: Когда все блоки разрушены (wallarray полностью очищен), игрок побеждает и видит сообщение "You Win!".
//    После этого игра перезапускается, сбрасывая счет и восстанавливая блоки.
//
// 4. **Звук**: Звуковые эффекты были улучшены с помощью функции `tone()` для разных состояний игры (удар по стенам, победа, поражение).
//
// Ссылки:
// - Код проекта на GitHub: hhttps://github.com/AIDevelopersMonster/arduino-lcd-keypad-shield-games/blob/master/sketches/Arduinoid/Arduinoid.ino
// - Видео по проекту на YouTube: https://youtu.be/6LWPz9LbFmI
// Written by Mason 2012

#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

#define MUSIC_PIN 3

// Constants for music timing
int bpm = 30;
const int whole = (60000 / bpm);
const int half = 30000 / bpm;
const int quarter = 15000 / bpm;
const int eight = 7500 / bpm;
const int sixteenth = 3750 / bpm;
const int thirty2 = 1875 / bpm;

float vballx = 1.0, vbally = 0.2, xball = 1.0, yball = 1.0;
int xmax = 80, delaytime = 60, score = 0, paddle_pos = 1;
int lcd_key = 0;
int adc_key_in = 0;
byte dot[8], paddle[8], wallFR[8], wallFL[8], wallBR[8], wallBL[8];
boolean wallarray[16] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

// Read button presses from LCD Keypad
int read_LCD_buttons() {
  adc_key_in = analogRead(0); // Read value from sensor
  if (adc_key_in > 1000) return btnNONE;  // No button pressed
  if (adc_key_in < 50) return btnRIGHT;
  if (adc_key_in < 195) return btnUP;
  if (adc_key_in < 380) return btnDOWN;
  if (adc_key_in < 555) return btnLEFT;
  if (adc_key_in < 790) return btnSELECT;
  return btnNONE;
}

// Get paddle position from button input
void getPaddle() {
  lcd_key = read_LCD_buttons();
  switch (lcd_key) {
    case btnUP:
      if (paddle_pos > 1) paddle_pos -= 1;
      break;
    case btnDOWN:
      if (paddle_pos < 14) paddle_pos += 1;
      break;
  }
}

// Draw walls
void drawwalls() {
  for (int i = 0; i < 8; i += 4) {
    for (int j = 0; j < 4; j++) {
      wallFL[j + i] = wallarray[i / 2] * 16 + wallarray[i / 2] * 8 + wallarray[i / 2 + 1] * 2 + wallarray[i / 2 + 1] * 1;
      wallFR[j + i] = wallarray[i / 2 + 4] * 16 + wallarray[i / 2 + 4] * 8 + wallarray[i / 2 + 5] * 2 + wallarray[i / 2 + 5] * 1;
      wallBL[j + i] = wallarray[i / 2 + 8] * 16 + wallarray[i / 2 + 8] * 8 + wallarray[i / 2 + 9] * 2 + wallarray[i / 2 + 9] * 1;
      wallBR[j + i] = wallarray[i / 2 + 12] * 16 + wallarray[i / 2 + 12] * 8 + wallarray[i / 2 + 13] * 2 + wallarray[i / 2 + 13] * 1;
    }
  }

  lcd.createChar(2, wallFL);
  lcd.createChar(3, wallFR);
  lcd.createChar(4, wallBL);
  lcd.createChar(5, wallBR);
  lcd.setCursor(14, 0);
  lcd.write((byte)2);
  lcd.write((byte)4);
  lcd.setCursor(14, 1);
  lcd.write((byte)3);
  lcd.write((byte)5);
}

// Place the dot (ball) on screen
void placedot(int x, int y) {
  createdot(x % 5, y % 8);
  lcd.setCursor(x / 5, y / 8);
  lcd.write((byte)0);
}

// Create the dot (ball) in 5x8 space
void createdot(int x, int y) {
  for (int i = 0; i < 8; i++) dot[i] = 0x0;
  if (y > 0) dot[y - 1] = (B11000 >> x);
  dot[y] = (B11000 >> x);
  lcd.createChar(0, dot);
}

// Place paddle on screen
void placepaddle(int y) {
  for (int i = 0; i < 8; i++) paddle[i] = 0x0;
  if (y % 8 > 0) paddle[y % 8 - 1] = 0x10;
  paddle[y % 8] = 0x10;
  if (y % 8 < 7) paddle[y % 8 + 1] = 0x10;
  lcd.createChar(1, paddle);
  lcd.setCursor(0, y / 8);
  lcd.write((byte)1);
}

// Handle ball-paddle-wall collisions
void handlecollisions() {
  xball += vballx;
  yball += vbally;

  // Handle Y-axis collisions
  if (yball > 15 || yball < 1) {
    vbally = -vbally;
    tone(MUSIC_PIN, 880, eight);
  }

  // Handle X-axis collisions with walls
  if (xball > 69) {
    for (int i = 0; i < 16; i++) {
      if (xball > (70 + 2 * (i % 2) + 5 * (i / 8))) {
        if ((yball > (2 * (i % 8))) && (yball < (2 * (i % 8) + 4))) {
          if (wallarray[i] == 1) {
            tone(MUSIC_PIN, 1174, eight);
            delay(eight);
            wallarray[i] = 0;
            vballx = -vballx;
            xball = 70;
          }
        }
      }
    }
  }

  // Check for screen boundaries
  if (xball > xmax) {
    vballx = -vballx;
    tone(MUSIC_PIN, 880, eight);
  }

  // Ball and paddle collision
  if (xball < 1) {
    if (paddle_pos > int(yball) - 2 && paddle_pos < int(yball) + 2) {
      tone(MUSIC_PIN, 1397, sixteenth);
      delay(sixteenth);
      tone(MUSIC_PIN, 1567, eight);
      vballx = -vballx;
      vbally *= random(1, 4);
      vbally /= 2;
      score++;
      delaytime -= 2;
      vballx += 0.1;

      // Check for win
      int left = 0;
      for (int i = 0; i < 16; i++) left += wallarray[i];
      if (left < 1) {
        lcd.clear();
        lcd.print("You Win!");
        arkanoidsong();
        lcd.print(score);
        delay(15000);
        xmax = 80;
        score = 0;
        delaytime = 60;
        for (int i = 0; i < 16; i++) wallarray[i] = 1;
      }
    } else {
      // Game Over logic
      gameOver();
    }
  }
}

// Game Over function
void gameOver() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Game Over");
  lcd.setCursor(0, 1);
  lcd.print("Score: ");
  lcd.print(score);
  
  tone(MUSIC_PIN, 440, whole);
  delay(whole);
  noTone(MUSIC_PIN);

  delay(2000); // Show "Game Over" for 2 seconds

  // Reset game state
  score = 0;
  xball = 1.0;
  yball = 1.0;
  vballx = 1.0;
  vbally = 0.2;
  paddle_pos = 1;
  for (int i = 0; i < 16; i++) wallarray[i] = 1;

  lcd.clear();
  lcd.print("Press to Restart");
  while (analogRead(0) > 1000) {
    delay(10);
  }
  delay(500);  // Give time for user to prepare for restart
}

// Play song (used for winning and some actions)
void arkanoidsong() {
  tone(MUSIC_PIN, 1568, eight); // g6
  delay(eight);
  noTone(MUSIC_PIN);
  delay(sixteenth);
  tone(MUSIC_PIN, 1568, sixteenth); // g6
  delay(sixteenth);
  tone(MUSIC_PIN, 1864, half); // a#6
  delay(half);
  noTone(MUSIC_PIN);
  delay(thirty2);
  tone(MUSIC_PIN, 1760, eight); // a6
  delay(eight);
  tone(MUSIC_PIN, 1568, eight); // g6
  delay(eight);
  tone(MUSIC_PIN, 1396, eight); // f6
  delay(eight);
  tone(MUSIC_PIN, 1760, eight); // a6
  delay(eight);
  tone(MUSIC_PIN, 1568, half);
  delay(half);
}

void setup() {
  lcd.begin(16, 2);
  delay(100);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Arduinoid");
  lcd.setCursor(0, 1);
  lcd.print("Get the Bricks");
  delay(500);
  arkanoidsong();
  delay(500);
  lcd.setCursor(0, 1);
  lcd.print("Press to Start");
  while (analogRead(0) > 1000) {
    delay(10);
  }
  Serial.begin(9600);
}

void loop() {
  lcd.clear();
  getPaddle();
  drawwalls();
  placepaddle(paddle_pos);
  handlecollisions();
  placedot(xball, yball);
  delay(delaytime);
}
