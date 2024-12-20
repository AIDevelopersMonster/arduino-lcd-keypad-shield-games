# Arduino Arkanoid Game

Это аркадная игра **Arkanoid**, созданная для работы с LCD Keypad Shield и использующая Arduino. В игре игрок управляет ракеткой, отражая мяч и разрушая блоки, стараясь набрать как можно больше очков. В проекте используются графика, звуковые эффекты и обработка столкновений.

## Описание

В этой игре игрок управляет ракеткой, которая перемещается вверх и вниз на экране с помощью кнопок на LCD Keypad Shield. Задача — отразить мяч и разрушить все блоки на экране. Игра включает в себя звуковые эффекты, отображение текущего счета, а также экраны "Game Over" и "You Win".

## Функции

- **Управление ракеткой** с помощью кнопок на LCD Keypad Shield:
  - **Up**: перемещает ракетку вверх.
  - **Down**: перемещает ракетку вниз.
  
- **Игровой процесс**:
  - Блоки исчезают, когда мяч сталкивается с ними.
  - Мяч отскакивает от ракетки и стен.
  - Когда мяч промахивается, появляется экран "Game Over", и игра перезапускается.
  - Когда все блоки разрушены, появляется экран "You Win!".

- **Звуковые эффекты** для столкновений, победы и поражения.

## Внесенные изменения

- **Game Over**: добавлен экран "Game Over", который появляется, когда мяч промахивается мимо ракетки. Игрок может начать заново.
- **Обработка промаха**: в случае промаха мяч сбрасывается в начальную позицию, и игра переходит в режим "Game Over".
- **Логика победы**: когда все блоки разрушены (wallarray полностью очищен), игрок побеждает, и появляется сообщение "You Win!".
- **Звук**: добавлены звуковые эффекты для различных игровых событий (удар по стенам, победа, поражение).
## Источник

Проект вдохновлен и частично основан на коде из [Arduinoid.ino](https://github.com/AIDevelopersMonster/arduino-lcd-keypad-shield-games/blob/master/sketches/Arduinoid/Arduinoid.ino) из репозитория `arduino-lcd-keypad-shield-games`. Внесены изменения для улучшения игровой логики, обработки промаха и победы.

## Установка

1. Подключите Arduino и LCD Keypad Shield.
2. Скачайте и установите библиотеку `LiquidCrystal` для работы с LCD экраном.
3. Скачайте исходный код проекта и загрузите его на вашу плату Arduino.



