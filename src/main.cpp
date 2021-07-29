#include "Arduino.h"
#include "InoBundle.h"
#include "games/Tetris.h"
#include "games/Demo.h"
#include "games/Snake.h"

Snake_t *snake;

Snake_t tempSnake;
void setup()
{
  snake = &tempSnake;
  Init();
  Start(snake);
}

void loop()
{
  TASK(READ_BUTTONS, ReadButtons(), 10);
  TASK(SNAKE_GAME, Update(snake), 250);
}