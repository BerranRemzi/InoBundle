#include "Arduino.h"
#include "InoBundle.h"

//#include "games/Tetris.h"
//#include "games/Demo.h"
#include "games/Snake.h"

Snake_t *snake;

void setup()
{
  snake = (Snake_t*)malloc(sizeof(Snake_t));
  Init();
  //Pixel(0,0, HIGH);
  //Pixel(7,7, HIGH);
  Start(snake);
}

void loop()
{
  TASK(INO_BUNDLE, InoBundle(), 10);
  TASK(SNAKE_GAME, Update(snake), 250);
}