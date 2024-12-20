/*
 *
 *      ____  ___   ________  ______    _   __   ____  ________  __________  __
 *     / __ \/   | / ____/  |/  /   |  / | / /  / __ \/  _/ __ \/ ____/ __ \/ /
 *    / /_/ / /| |/ /   / /|_/ / /| | /  |/ /  / /_/ // // / / / __/ / /_/ / / 
 *   / ____/ ___ / /___/ /  / / ___ |/ /|  /  / _, _// // /_/ / /___/ _, _/_/  
 *  /_/   /_/  |_\____/_/  /_/_/  |_/_/ |_/  /_/ |_/___/_____/_____/_/ |_(_)   
 *
 * Written by Micky Griffiths for the 16x2 LCD and RPi. -- 16 May 2013
 * Ported to Arduino for the Arduino LCD Keypad Shield by Johannes le Roux (@dadecoza) -- 4 Apr 2018
 * Updated to include sound effects by Alex Malachevsky 20/12/2024
 * Heart collection: When Pacman eats a heart, a "tune" sound effect is played.
 *  Ghost collision: When Pacman collides with a ghost, a "collision" sound is played.
 *  Movement sound: A short tone is played every time Pacman moves.
 *  Winning or specific actions: A short song (arkanoidsong) is played during the intro or upon certain actions like completing a round.
 *  Sound Timing: Constants like whole, half, quarter and others were defined for timing and music control, helping to make the sound effects consistent and easily adjustable based on the beats per minute (bpm).
 */

#include <LiquidCrystal.h>   // Подключение библиотеки для работы с LCD
#include <EEPROM.h>          // Подключение библиотеки для работы с EEPROM

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);  // Инициализация объекта для работы с LCD экраном

static const int BUZZER_PIN = 3; // Пин для пьезодинамика
bool triggerMode = true;  // false - High Trigger(npn), true - Low Trigger(pnp)

// Constants for music timing
int bpm = 30; // Темп музыки в ударах в минуту (bpm)
const int whole = (60000 / bpm);        // Продолжительность целого такта
const int half = 30000 / bpm;           // Продолжительность половины такта
const int quarter = 15000 / bpm;        // Продолжительность четверти такта
const int eight = 7500 / bpm;           // Продолжительность восьмой доли
const int sixteenth = 3750 / bpm;       // Продолжительность шестнадцатой доли
const int thirty2 = 1875 / bpm;         // Продолжительность тридцать второй доли

static const int SPRITE_PACMAN_OPEN = 0;
static const int SPRITE_PACMAN_CLOSED = 1;
static const int SPRITE_GHOST = 2;
static const int SPRITE_HEART = 3;
static const int SPRITE_SMILEY = 4;
static const int TYPE_NONE = 0;
static const int TYPE_HEART = 1;
static const int TYPE_GHOST = 2;
static const int MAX_SPRITES = 10;       // Максимальное количество спрайтов на экране
static const int STATE_INTRO = 0;        // Состояние игры - Введение
static const int STATE_PLAY = 1;         // Состояние игры - Игровой процесс
static const int STATE_GAMEOVER = 2;     // Состояние игры - Игра завершена
static const int HIGHSCORE_ADDRESS = 0;  // Адрес для хранения рекорда в EEPROM
static const int TOP = 0;                // Верхняя строка на экране
static const int BOTTOM = 1;             // Нижняя строка на экране

// Структура для хранения данных о спрайте
struct sprite {
  int x;       // Координата X спрайта
  int y;       // Координата Y спрайта
  int type;    // Тип спрайта (пакман, призрак, сердце и т.д.)
};

struct sprite sprites[MAX_SPRITES];  // Массив спрайтов

long timeToMove, timeToAnimate, timeToDebounce;
int state, score, highScore, gameSpeed, pacmanX, pacmanY, ghostOdds;
boolean mouthState, smile;

static const byte spriteBitmaps[8][8] = { // Пиксельные изображения спрайтов
  {0x7, 0xF, 0x1E, 0x1C, 0x1C, 0x1E, 0xF, 0x7},  // Пакман с открытым ртом
  {0x0, 0xF, 0x1F, 0x1F, 0x1E, 0x1F, 0xF, 0x0},  // Пакман с закрытым ртом
  {0x19, 0x1F, 0x15, 0x1F, 0x11, 0x1F, 0x1D, 0xC},  // Призрак
  {0x0, 0x0, 0x0, 0xA, 0x15, 0x11, 0xA, 0x4},  // Сердце
  {0x0, 0xA, 0x0, 0x0, 0x11, 0xE, 0x0, 0x0},  // Улыбка
  {0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x0, 0x0, 0x0},  // Ничего
  {0x0, 0x0, 0x0, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F},  // Призрак в движении
  {0x1F, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}   // Сердце в движении
};

// Функция для воспроизведения песни (при победе или некоторых действиях)
void arkanoidsong() {
  tone(BUZZER_PIN, 1568, eight); // g6
  delay(eight);
  noTone(BUZZER_PIN);
  digitalWrite(BUZZER_PIN, triggerMode ? HIGH : LOW);  
  delay(sixteenth);
  tone(BUZZER_PIN, 1568, sixteenth); // g6
  delay(sixteenth);
  tone(BUZZER_PIN, 1864, half); // a#6
  delay(half);
  noTone(BUZZER_PIN);
  digitalWrite(BUZZER_PIN, triggerMode ? HIGH : LOW);  
  delay(thirty2);
  tone(BUZZER_PIN, 1760, eight); // a6
  delay(eight);
  tone(BUZZER_PIN, 1568, eight); // g6
  delay(eight);
  tone(BUZZER_PIN, 1396, eight); // f6
  delay(eight);
  tone(BUZZER_PIN, 1760, eight); // a6
  delay(eight);
  tone(BUZZER_PIN, 1568, half);
  delay(half);
  noTone(BUZZER_PIN);
  digitalWrite(BUZZER_PIN, triggerMode ? HIGH : LOW);  
}

// Инициализация переменных при старте игры
void setup() {
  pinMode(BUZZER_PIN, OUTPUT); // Настройка пина для пьезодинамика
  digitalWrite(BUZZER_PIN, triggerMode ? HIGH : LOW); 
  
  // Если кнопка нажата, сбрасываем рекорд в EEPROM
  if (checkButton()) {
    EEPROM.write(HIGHSCORE_ADDRESS, 0);
  }

  // Создание пользовательских символов для LCD
  for (int i = 0; i < (sizeof(spriteBitmaps) / 8); i++) {
    lcd.createChar(i, spriteBitmaps[i]);
  }

  delay(500);
  lcd.begin(16, 2);  // Инициализация LCD экрана
  randomSeed(analogRead(1));  // Инициализация случайных чисел
  state = STATE_INTRO; // Установка начального состояния
  initVars();  // Инициализация переменных игры
}

void loop() {
  // В зависимости от состояния вызывается соответствующая функция
  switch (state) {
    case STATE_INTRO: intro(); break;  // Вводная заставка
    case STATE_PLAY: play(); break;    // Игра
    case STATE_GAMEOVER: gameover(); break; // Конец игры
  }
}

void initVars() {
  // Инициализация всех переменных
  for (int i = 0; i < MAX_SPRITES; i++) {
    sprites[i] = {0, 0, TYPE_NONE};  // Очищаем все спрайты
  }
  
  // Чтение максимального счета из памяти
  highScore = EEPROM.read(HIGHSCORE_ADDRESS);
  
  // Инициализация других переменных
  timeToMove = 0;
  timeToAnimate = 0;
  timeToDebounce = 0;
  score = 0;
  gameSpeed = 600;  // Скорость игры
  ghostOdds = 6;    // Шанс появления призраков
  pacmanX = 1;      // Начальная позиция Pacman по оси X
  pacmanY = 0;      // Начальная позиция Pacman по оси Y
  mouthState = false; // Состояние рта Pacman
  smile = false;      // Улыбка Pacman
}

void intro() {
  // Вводная заставка, показывающая информацию игроку
  
  lcd.clear(); 
  lcd.setCursor(3, 0); lcd.print("WELCOME TO");  // Приветствие
  lcd.setCursor(1, 1); lcd.print("MICKY'S ARCADE"); // Название игры
  arkanoidsong();  // Проигрывание звукового вступления
  waitButton();  // Ожидание нажатия кнопки для продолжения

  lcd.clear(); 
  lcd.setCursor(2, 0); lcd.print("IT'S SIMPLE!");  // Пояснение, что игра простая
  waitButton();

  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("Press the button");  // Указание на действия
  lcd.setCursor(1, 1); lcd.print("to move pacman");
  waitButton();

  lcd.clear();
  lcd.setCursor(6, 0); lcd.print("BUT!");  // Важное предупреждение
  lcd.setCursor(0, 1); lcd.print("Don't forget to");
  waitButton();

  lcd.clear();
  lcd.setCursor(3, 0); lcd.print("COLLECT: ");  // Собирайте сердечки
  lcd.write(byte(SPRITE_HEART));  // Символ сердечка
  lcd.setCursor(4, 1); lcd.print("AVOID: ");  // Избегайте призраков
  lcd.write(byte(SPRITE_GHOST));  // Символ призрака
  waitButton();

  lcd.clear();
  lcd.setCursor(1, 0); lcd.print("ARE YOU READY?");  // Подготовка к началу игры
  lcd.setCursor(0, 1); lcd.print("Press the button");
  waitButton();

  animation(1);  // Анимация начала игры
  state = STATE_PLAY;  // Переход в состояние игры
}

void play() {
  // Отображение игрового экрана
  drawScreen();
  
  // Получаем текущее время с момента включения
  long now = millis();
  
  // Проверка нажатия кнопки для изменения состояния Pacman
  if (checkButton()) {
    hidePacman();  // Скрыть текущего Pacman
    pacmanY = !pacmanY;  // Переключаем позицию Pacman по оси Y (вверх/вниз)
  }

  // Проверка, прошло ли время для перемещения
  if (now > timeToMove) {
    moveLeft();  // Перемещаем все объекты на экране влево

    // С вероятностью, зависящей от ghostOdds, спавним призрака
    if (!random(0, ghostOdds)) {
      spawn(TYPE_GHOST);
    }

    // С вероятностью 1 из 3 спавним сердечко
    if (!random(0, 3)) {
      spawn(TYPE_HEART);
    }

    // Обновляем время следующего перемещения
    timeToMove = now + gameSpeed;
  }

  // Проверка на столкновение с объектом
  int c = collision();
  if (c == TYPE_HEART) {
    // Если столкновение с сердечком
    eatHeart();  // Съедаем сердечко
    increaseScore();  // Увеличиваем счет
    tone(BUZZER_PIN, 1000, 200);  // Звук при сборе сердечка
    delay(200);
    noTone(BUZZER_PIN);  // Отключаем звук
    digitalWrite(BUZZER_PIN, triggerMode ? HIGH : LOW);  
  } else if (c == TYPE_GHOST) {
    // Если столкновение с призраком
    tone(BUZZER_PIN, 200, 500);  // Звук при столкновении с призраком
    delay(500);  // Ожидаем завершения звука
    noTone(BUZZER_PIN);  // Отключаем звук
    digitalWrite(BUZZER_PIN, triggerMode ? HIGH : LOW);  
    state = STATE_GAMEOVER;  // Переход к состоянию "Конец игры"
  }
}

void gameover() {
  // Анимация окончания игры
  animation(0);

  // Отображаем сообщение о конце игры
  lcd.setCursor(3, 0); lcd.print("GAME OVER");
  lcd.setCursor(0, 1); lcd.print("How did you do?");
  waitButton();  // Ожидаем нажатия кнопки для продолжения

  // Отображаем текущий счет
  lcd.clear(); 
  lcd.setCursor(2, 0); lcd.print("YOUR SCORE:");
  lcd.setCursor(7, 1); lcd.print(score);
  waitButton();  // Ожидаем нажатия кнопки для продолжения

  // Если новый рекорд, сохраняем его в памяти
  if (score > highScore) {
    lcd.clear(); 
    lcd.setCursor(1, 0); lcd.print("NEW HIGHSCORE!");
    EEPROM.write(HIGHSCORE_ADDRESS, score);  // Записываем новый рекорд в EEPROM
    highScore = score;  // Обновляем значение рекорда
    waitButton();  // Ожидаем нажатия кнопки для продолжения
  }

  // Отображаем рекорд
  lcd.clear(); 
  lcd.setCursor(3, 0); lcd.print("TRY AGAIN");
  lcd.setCursor(2, 1); lcd.print("Highscore: ");
  lcd.print(highScore);
  waitButton();  // Ожидаем нажатия кнопки для продолжения

  // Инициализация переменных для новой игры
  initVars();
  state = STATE_PLAY;  // Возвращаемся к игровому процессу
  lcd.clear();  // Очищаем экран
}

void drawScreen() {
  for (int i = 0; i < MAX_SPRITES; i++) {
    drawSprite(i);  // Отрисовываем каждый спрайт
  }
  drawPacman();  // Отрисовываем Pacman
}
void eatHeart() {
  for (int i = 0; i < MAX_SPRITES; i++) {
    if (sprites[i].x == pacmanX && sprites[i].y == pacmanY && sprites[i].type == TYPE_HEART) {
      smile = true;  // Устанавливаем флаг для улыбки Pacman
      deleteSprite(i);  // Удаляем съеденное сердечко
      return;
    }
  }
}
void increaseScore() {
  score++;  // Увеличиваем счет
  if (!(score % 10)) {
    // Каждые 10 очков уменьшаем скорость игры и меняем вероятность появления призраков
    gameSpeed = gameSpeed - 30;
    if (ghostOdds > 1) {
      ghostOdds--;
    }
  }
}
void spawn(int type) {
  int x = 15;  // Спавним объекты справа
  int y = random(0, 2);  // Выбираем случайную позицию (верхнюю или нижнюю строку)
  for (int i = 0; i < MAX_SPRITES; i++) {
    int spriteType = sprites[i].type;
    if (spriteType == TYPE_NONE) {
      if (((type == TYPE_GHOST) && okayToSpawnGhost(y)) || ((type == TYPE_HEART) && okayToSpawnHeart(y))) {
        createSprite(i, x, y, type);  // Если можно спавнить объект, создаем его
      }
      return;
    }
  }
}


int at(int x, int y) {
  for (int i = 0; i < MAX_SPRITES; i++) {
    if (sprites[i].x == x && sprites[i].y == y && sprites[i].type != TYPE_NONE) {
      return sprites[i].type;
    }
  }
  return TYPE_NONE;
}


int okayToSpawnGhost(int pos) {
  if (at(15, pos) != TYPE_NONE) {
    return 0;  // Если позиция занята, нельзя спавнить
  } else if (at(15, !pos) == TYPE_GHOST) {
    return 0;  // Если напротив уже есть призрак, не спавним
  } else if ((pos == TOP) && (at(14, BOTTOM) == TYPE_GHOST)) {
    return 0;  // Если внизу есть призрак, не спавним сверху
  } else if ((pos == BOTTOM) && (at(14, TOP) == TYPE_GHOST)) {
    return 0;  // Если сверху есть призрак, не спавним снизу
  }
  return 1;  // Можно спавнить призрака
}


int okayToSpawnHeart(int pos) {
  if (at(15, pos) != TYPE_NONE) {
    return 0;  // Если в позиции (15, pos) уже есть объект, нельзя спавнить сердечко
  }
  return 1;  // Можно спавнить сердечко
}

void moveLeft() {
  for (int i = 0; i < MAX_SPRITES; i++) {
    if (sprites[i].type != TYPE_NONE) {
      int x = sprites[i].x - 1;  // Двигаем объект на 1 влево
      int y = sprites[i].y;
      moveSprite(i, x, y);  // Обновляем позицию спрайта
      tone(BUZZER_PIN, 600, 50); // Издаём звук при движении
    }
  }
  noTone(BUZZER_PIN);  // Отключаем звук
  digitalWrite(BUZZER_PIN, triggerMode ? HIGH : LOW);  
}

void createSprite(int s, int x, int y, int type) {
  sprites[s].x = x;
  sprites[s].y = y;
  sprites[s].type = type;
}

void drawSprite(int s) {
  int spriteType = sprites[s].type;
  if (spriteType == TYPE_NONE) {
    return;  // Если спрайт пустой, ничего не рисуем
  }
  int x = sprites[s].x;
  int y = sprites[s].y;
  if (!((x == pacmanX) && (y == pacmanY))) {
    lcd.setCursor(x, y);  // Устанавливаем курсор для отрисовки спрайта
    switch (spriteType) {
      case TYPE_GHOST: lcd.write(byte(SPRITE_GHOST)); break;  // Рисуем призрака
      case TYPE_HEART: lcd.write(byte(SPRITE_HEART)); break;  // Рисуем сердечко
      default: lcd.write(byte(32)); break;  // Рисуем пустое пространство (если тип неизвестен)
    }
  }
}

void hideSprite(int s) {
  lcd.setCursor(sprites[s].x, sprites[s].y);  // Устанавливаем курсор в позицию спрайта
  lcd.write(byte(32));  // Очищаем место, заменяя спрайт на пустое пространство
}

void drawPacman() {
  if ((millis() > timeToAnimate) || (smile)) {
    int wait = 350;  // Время ожидания для анимации
    lcd.setCursor(pacmanX, pacmanY);
    if (smile) {
      lcd.write(byte(SPRITE_SMILEY));  // Рисуем улыбающийся Pacman
      wait = 600;  // Увеличиваем время ожидания для анимации
      smile = false;  // Сброс флага улыбки
    } else if (mouthState) {
      lcd.write(byte(SPRITE_PACMAN_OPEN));  // Рисуем открытый рот Pacman
    } else {
      lcd.write(byte(SPRITE_PACMAN_CLOSED));  // Рисуем закрытый рот Pacman
    }
    mouthState = !mouthState;  // Переключаем состояние рта
    timeToAnimate = millis() + wait;  // Устанавливаем время следующей анимации
  }
}

void hidePacman() {
  lcd.setCursor(pacmanX, pacmanY);  // Устанавливаем курсор в позицию Pacman
  lcd.write(byte(32));  // Очищаем место, заменяя Pacman на пустое пространство
}

void deleteSprite(int s) {
  hideSprite(s);  // Скрыть спрайт
  sprites[s].x = 0;  // Сбросить координаты спрайта
  sprites[s].y = 0;
  sprites[s].type = TYPE_NONE;  // Установить тип как пустой
}


void moveSprite(int s, int x, int y) {
  if ((x < 0) || (x > 15)) {
    deleteSprite(s);  // Если координаты выходят за границы экрана, удалить спрайт
  } else {
    hideSprite(s);  // Скрыть текущую позицию спрайта
    sprites[s].x = x;  // Обновить координаты спрайта
    sprites[s].y = y;
  }
}


int collision() {
  return at(pacmanX, pacmanY);  // Проверка на столкновение Pacman с объектами на его текущей позиции
}


void animation(int direction) {
  byte animationOpen[6] = {255, 255, 5, 6, 7, 95};  // Анимация для открытия
  byte animationClose[6] = {7, 95, 5, 6, 255, 255};  // Анимация для закрытия
  byte animationChars[6];
  
  if (direction) {
    memcpy(animationChars, animationOpen, sizeof animationChars);  // Если направление открывается
  } else {
    memcpy(animationChars, animationClose, sizeof animationChars);  // Если направление закрывается
  }
  
  lcd.clear();
  for (int frame = 0; frame < 3; frame++) {
    lcd.setCursor(0, 0);
    for (int i = 0; i < 16; i++) lcd.write(animationChars[frame * 2]);  // Отрисовать первую строку анимации
    lcd.setCursor(0, 1);
    for (int i = 0; i < 16; i++) lcd.write(animationChars[(frame * 2) + 1]);  // Отрисовать вторую строку анимации
    delay(300);  // Задержка между кадрами
    lcd.clear();
  }
}


int checkButton() {
  long now = millis();
  if (now > timeToDebounce) {  // Если прошло время для дебаунса
    int b = analogRead(A0);  // Считываем значение с аналогового пина A0
    if (b < 850) {  // Если значение кнопки меньше порогового
      timeToDebounce = now + 300;  // Устанавливаем время для следующей проверки
      return 1;  // Кнопка нажата
    }
  }
  return 0;  // Кнопка не нажата
}



void waitButton() {
  while (!checkButton()) delay(50);  // Ждать, пока не будет нажата кнопка
}
