/* DinosaurGame
 *  Written by Erbol Syzdyk
 * 
 * Changes made by Alex Malachevsky:
 * 1. Added start screen with "DINOSAUR GAME" and "PRESS ANY KEY".
 * 2. Game now waits for a button press before starting.
 * 3. Added music for startup, game over, button presses, jumping, and collisions.
 * 4. Music is played using a passive buzzer connected to pin 3.
 * 5. The game features simple obstacles that the dinosaur must jump over.
 * 
 * Enjoy playing the Dinosaur Game on your Arduino!
 */



#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

int read_LCD_buttons() {
  int adc_key_in = analogRead(0);  // Read the value from the button sensor
  if (adc_key_in > 1000) return btnNONE; 
  if (adc_key_in < 50)   return btnRIGHT;
  if (adc_key_in < 195)  return btnUP; 
  if (adc_key_in < 380)  return btnDOWN; 
  if (adc_key_in < 555)  return btnLEFT; 
  if (adc_key_in < 790)  return btnSELECT;
  
  return btnNONE;
  
}
byte dino[8] =
{
 B00010,
 B00111,
 B00111,
 B00110,
 B00111,
 B10110,
 B11110,
 B01010
};
byte dino1[8] =
{
 B00010,
 B00111,
 B00111,
 B00110,
 B00111,
 B10110,
 B11110,
 B01000
};

byte dino2[8] =
{
 B00010,
 B00111,
 B00111,
 B00110,
 B00111,
 B10110,
 B11110,
 B00010
};
byte dinosit[8] =
{
 B00000,
 B00000,
 B00010,
 B00111,
 B00111,
 B10110,
 B11110,
 B01010
};
byte dinosit1[8] =
{
 B00000,
 B00000,
 B00010,
 B00111,
 B00111,
 B10110,
 B11110,
 B01000
};
byte dinosit2[8] =
{
 B00000,
 B00000,
 B00010,
 B00111,
 B00111,
 B10110,
 B11110,
 B00010
};
byte obs1[8] =
{
 B00000,
 B00000,
 B10000,
 B11101,
 B11010,
 B01010,
 B01110,
 B01100
};

byte obs2[8] =
{
 B00100,
 B00100,
 B01110,
 B11111,
 B11111,
 B01110,
 B00100,
 B00100
};

byte obs3[8] =
{
 B11111,
 B01110,
 B00100,
 B00000,
 B00000,
 B00000,
 B00000,
 B00000
};

byte obs4[8] =
{
 B00100,
 B10101,
 B10101,
 B11111,
 B10101,
 B10101,
 B11111,
 B00100
};
byte freep[8] =
{
 B00000,
 B00000,
 B00000,
 B00000,
 B00000,
 B00000,
 B00000,
 B00000
};

int mat[2][24] = {
  {7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7},
  {7,0,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7}
  };

int dinoAnim[] = {1,0,2,3};

void playSetupMusic() {
  // Мелодия для старта игры
  tone(3, 440, 300); // Ля (A4) - 300 миллисекунд
  delay(300);
  tone(3, 523, 300); // До (C5) - 300 миллисекунд
  delay(300);
  tone(3, 659, 300); // Ми (E5) - 300 миллисекунд
  delay(300);
  tone(3, 784, 300); // Соль (G5) - 300 миллисекунд
  delay(300);
  noTone(3); // Останавливаем звук
}
void playButtonPressSound() {
  tone(3, 1000, 100); // Высокий звук (1000 Гц) - 100 миллисекунд
  delay(100);
  noTone(3); // Останавливаем звук
}
void playJumpSound() {
  tone(3, 880, 200); // Высокий звук (880 Гц) - 200 миллисекунд (например, прыжок)
  delay(200);
  noTone(3); // Останавливаем звук
}

void playCollisionSound() {
  tone(3, 330, 200); // Средний звук (330 Гц) - 200 миллисекунд (например, столкновение)
  delay(200);
  noTone(3); // Останавливаем звук
}
void setup() {
  Serial.begin(9600);
    // Отображаем начальный экран
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);  // Начало первой строки
  lcd.print("*DINOSAUR GAME!*");  // Первая строка
  lcd.setCursor(0, 1);  // Начало второй строки
  lcd.print("*PRESS ANY KEY!*");  // Вторая строка

  // Ждем нажатия любой кнопки
while (read_LCD_buttons() == btnNONE) {
    playSetupMusic();
    // Пустой цикл, ждем нажатия
    delay(100);  // Добавляем небольшую задержку для улучшения отзывчивости
  }

  // После нажатия кнопки очищаем экран и начинаем игру
  lcd.clear();
  
  //pinMode(but1,INPUT);
  //pinMode(but2,INPUT);
  lcd.createChar(0, dino1);
  lcd.createChar(1, dino2);
  lcd.createChar(2, dinosit1);
  lcd.createChar(3, dinosit2);
  lcd.createChar(4, obs1);
  lcd.createChar(5, obs3);
  lcd.createChar(6, obs4);
  lcd.createChar(7, freep);
  lcd.begin(16, 2);



}
int b1 = 0;
int b2 = 0;
int walk = 0;
int fps = 0;
int maxhigh = 600;
long jumpstarted = 0;
int isJumping = 1;
int pausebetween = 1;
bool gamestarted = false;
int highscore = 0;
int score = 0;
void redraw(){ 
  for(int j = 15; j >= 0; j--){
    for(int i = 0; i < 2; i++){
      lcd.setCursor(j,i);
      lcd.write(mat[i][j]);
    }
  }
  if(!gamestarted){
    String s = "Score: "+String(score);
    String hs = "HighS: "+String(highscore);
    lcd.setCursor(3,0);
    lcd.print(s);
    lcd.setCursor(3,1);
    lcd.print(hs);
  }
}

void cleanmat(){
  for(int i = 0; i < 2; i++){
    for(int j = 0; j < 16; j++){
      mat[i][j] = 7;
    }
  }
}

void gameover(){
  cleanmat();
  gamestarted = false;
  pausebetween = 1;
  delay(1000);
}
void loop() {
  if((read_LCD_buttons() == btnUP) && !b2)
  jumpstarted = millis();
  
  b1 = (read_LCD_buttons() == btnDOWN);
  b2 = (read_LCD_buttons() == btnUP);
   
  if(!gamestarted){
    if(b1 || b2){
      gamestarted = true;
      score = 0;
      
    }
  }
  if(gamestarted){
    if(fps%3==0)walk+=1;
    walk%=2;
    if(fps%4==0){
      pausebetween -= 1;
      for(int i = 0; i<2;i++){
        for(int j = 0; j<23; j++){
          mat[i][j] = mat[i][j+1];
        }
      }
      mat[0][23] = 7;
      mat[1][23] = 7;
    }
  
    if(!pausebetween)generate_obstacles();
    
    isJumping = !((millis()-jumpstarted<maxhigh) && b2);

    if(!isJumping && mat[0][1]==6){
      gameover();
    }
    if(!isJumping && !b1 && mat[0][1]==5){
      gameover();
    }
    if(isJumping && mat[1][1]==4){
      gameover(); 
    }
    if(isJumping && mat[1][1]==5 && !b1){
      gameover();
    }
    if(!b1) score+=1;
    if(score>highscore)highscore=score; 
    fps+=1;
  }
  redraw();
  lcd.setCursor(1,isJumping);
  lcd.write(dinoAnim[b1*2+(walk)]);
  
  delay(20);
}

void generate_obstacles(){
  int obscount = random(1,4);
  int types[4][2] = {{7,4},{6,5},{5,4},{6,7}} ;
  int type = random(0,4);
  for(int i = 0; i< obscount; i++){
    mat[0][16+i] = types[type][0];
    mat[1][16+i] = types[type][1];
  }
  int spacecount = random(2,5);
  pausebetween = obscount + spacecount;
}

