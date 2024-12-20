// Pacman game for Arduino with LCD Keypad Shield
// Author: Jean Malha
// Modifications by Alex Malachevsky: Added sound effects and buzzer functionality
// GitHub: https://github.com/AIDevelopersMonster/arduino-lcd-keypad-shield-games/tree/master/sketches/Pacman
// Video demonstration: https://youtube.com/shorts/Z0TSHcleV8Y
// Features: Pacman movement, ghost chasing, level progression, and point collection with added sound feedback


#include <LiquidCrystal.h> // Library for LCD control

// Game parameters
#define VITESSE_PAC 150  // Pacman movement speed in ms / Скорость движения Пакмана (мс)
#define VITESSE_FANT 2000 // Ghost movement speed in ms / Скорость движения призрака (мс)
#define MAXX 15          // Maximum X coordinate / Максимальная координата X
#define MAXY 1           // Maximum Y coordinate / Максимальная координата Y
static int speakerPin = 3;
bool triggerMode = true;  // false - High Trigger(npn), true - Low Trigger(pnp)
// Constants for music timing
int bpm = 30;
const int whole = (60000 / bpm);
const int half = 30000 / bpm;
const int quarter = 15000 / bpm;
const int eight = 7500 / bpm;
const int sixteenth = 3750 / bpm;
const int thirty2 = 1875 / bpm;
// Button definitions / Определение кнопок
#define btnRight     0
#define btnUp        1
#define btnDown      2
#define btnLeft      3
#define btnSelect    4
#define btnNone      5

void(* resetFunc) (void) = 0; // Reset function / Функция сброса

LiquidCrystal lcd(8, 9, 4, 5, 6, 7); // LCD pin configuration / Подключение ЖК-дисплея

// Custom characters for Pacman and ghost / Специальные символы для Пакмана и призрака
byte pacman[8] = {
  B00000,
  B00000,
  B01110,
  B11011,
  B11100,
  B01110,
  B00000,
  B00000
};

byte fantome[8] = {
  B00000,
  B00000,
  B01110,
  B10101,
  B11111,
  B11111,
  B10101,
  B00000
};

byte point[8] = { // Points to eat / Точки для поедания
  B00000,
  B00000,
  B00000,
  B01110,
  B01110,
  B00000,
  B00000,
  B00000
};

// Board to track points / Поле для отслеживания точек
byte points[MAXX+1][MAXY+1];

// Game state variables / Переменные состояния игры
int xpac=2;  // Pacman's X position / Позиция Пакмана по X
int ypac=1;  // Pacman's Y position / Позиция Пакмана по Y
int xfant=15;// Ghost's X position / Позиция призрака по X
int yfant=0; // Ghost's Y position / Позиция призрака по Y
byte light=true; // Backlight state / Состояние подсветки
long keystruck=0; // Last button press time / Время последнего нажатия кнопки
long poursuite=0; // Last ghost movement / Время последнего движения призрака
byte partieEnCours=true; // Game running flag / Флаг текущей игры
byte vide=false; // Check if all points eaten / Проверка, все ли точки съедены

byte level=0; // Current level / Текущий уровень
int score=0;  // Current score / Текущий счет

// Function to move Pacman / Функция для перемещения Пакмана
void bouge(int x, int y) {
  int oldx=xpac;
  int oldy=ypac;
  if (((xpac+x)>=0)&((xpac+x)<=MAXX)) xpac=xpac+x; // Move if within bounds / Движение, если в пределах экрана
  if (((ypac+y)>=0)&((ypac+y)<=MAXY)) ypac=ypac+y; // Move if within bounds / Движение, если в пределах экрана
  lcd.setCursor(xpac,ypac); 
  lcd.write(byte(0)); // Draw Pacman / Отрисовка Пакмана
  lcd.setCursor(oldx,oldy); 
  if ((xpac!=oldx)||(ypac!=oldy)) lcd.print(" "); // Clear old position / Очистка старой позиции
  if(points[xpac][ypac]){
    points[xpac][ypac]=false; // Eat point / Поедание точки
    score++;
  }
  vide=true;
  for (int i=0; i<=MAXX; i=i+1)
    for (int j=0; j<=MAXY; j=j+1)
      if (points[i][j]) vide=false;
  if ((vide)&&(partieEnCours)) gagne();
}

// Function called when player loses / Функция при проигрыше
void perdu() {
  lcd.setCursor(0, 0); 
  lcd.print("***Game Over****"); 
  lcd.setCursor(0, 1); 
  lcd.print("***");
  lcd.print(score);
  lcd.print("***"); 
  delay(2000);
  resetFunc();
}

// Function called when player wins / Функция при победе
void gagne() {
  level++;
  lcd.setCursor(0, 0); 
  lcd.print("*** Next level ***"); 
  lcd.setCursor(0, 1); 
  lcd.print("*** ");
  lcd.print(level,DEC);
  lcd.print(" ***"); 
  delay(2000); 
  initLevel(); // Reset level / Сброс уровня
}

// Function to move the ghost / Функция для движения призрака
void poursuis() {
  int oldx=xfant;
  int oldy=yfant;
      tone(speakerPin, 1174, thirty2);
            delay(thirty2);
            noTone(speakerPin);
            digitalWrite(speakerPin, triggerMode ? HIGH : LOW);  
  if (yfant<ypac) yfant=yfant+1;
  else if (yfant>ypac) yfant=yfant-1;
  else if (xfant<xpac) xfant=xfant+1;
  else if (xfant>xpac) xfant=xfant-1;
  lcd.setCursor(xfant,yfant); 
  lcd.write(1); 
  lcd.setCursor(oldx,oldy); 
  if ((oldx!=xfant)||(oldy!=yfant)) {
    if (points[oldx][oldy]) lcd.write(2); 
    else lcd.print(" ");
     
  }
}

// Level initialization / Инициализация уровня
void initLevel() {
  for (int i=0; i<=MAXX; i=i+1)
    for (int j=0; j<=MAXY; j=j+1) {
      points[i][j]=true; 
      lcd.setCursor(i-1, j-1); 
      lcd.write(2); 
    }
  lcd.setCursor(xpac,ypac); 
  lcd.write(byte(0)); 
  lcd.setCursor(xfant,yfant); 
  lcd.write(1); 
  poursuite=millis(); 
  vide=false;
}
// Play song (used for winning and some actions)
void arkanoidsong() {
  tone(speakerPin, 1568, eight); // g6
  delay(eight);
  noTone(speakerPin);
  digitalWrite(speakerPin, triggerMode ? HIGH : LOW);  
  delay(sixteenth);
  tone(speakerPin, 1568, sixteenth); // g6
  delay(sixteenth);
  tone(speakerPin, 1864, half); // a#6
  delay(half);
  noTone(speakerPin);
  digitalWrite(speakerPin, triggerMode ? HIGH : LOW);  
  delay(thirty2);
  tone(speakerPin, 1760, eight); // a6
  delay(eight);
  tone(speakerPin, 1568, eight); // g6
  delay(eight);
  tone(speakerPin, 1396, eight); // f6
  delay(eight);
  tone(speakerPin, 1760, eight); // a6
  delay(eight);
  tone(speakerPin, 1568, half);
  delay(half);
  noTone(speakerPin);
  digitalWrite(speakerPin, triggerMode ? HIGH : LOW);  
}
// Setup function / Функция настройки
void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.createChar(0, pacman);
  lcd.createChar(1, fantome);
  lcd.createChar(2, point);
  lcd.setCursor(0, 0); 
  lcd.print("     PACMAN"); 
    arkanoidsong();
  delay(5000); 
  initLevel();
}

// Main game loop / Главный игровой цикл
void loop() {
  int thisChar = Serial.read();
  switch (thisChar) {
  case 'r':
    lcd.scrollDisplayRight();
    break;
  case 'l':
    lcd.scrollDisplayLeft();
    break;
  }
  if (millis()-keystruck>VITESSE_PAC) {
    int joy=getKey();
    switch (joy) {
    case btnNone:
      break;
    case btnLeft:
      bouge(-1,0);
      keystruck=millis();
      break;
    case btnRight:
      bouge(1,0);
      keystruck=millis();
      break;
    case btnUp:
      bouge(0,-1);
      keystruck=millis();
      break;
    case btnDown:
      bouge(0,1);
      keystruck=millis();
      break;
    default:
      keystruck=millis();
    }
  }
  if (millis()-poursuite>VITESSE_FANT/(level+1)+10) {
    poursuis();
    poursuite=millis();
  }
  if ((xpac==xfant)&&(ypac==yfant)&&(partieEnCours)) {
    perdu();
  }
}

// Function to read input from joystick / Функция для считывания ввода с джойстика
int getKey() {
  int b = analogRead(A0);
  if (b > 1000) return btnNone;
  delay(8);
  if (b < 50) return btnRight;
  if (b < 180) return btnUp;
  if (b < 330) return btnDown;
  if (b < 520) return btnLeft;
  if (b < 700) return btnSelect;
}
