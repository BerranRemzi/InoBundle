#include "Arduino.h"
#include "InoBundle.h"

//#include "games/Tetris.h"
//#include "games/Demo.h"
#include "games/Snake.h"

Snake_t *snake = new Snake_t;

extern uint16_t adcValue[2];
void Callback()
{
  Serial.print(adcValue[0]);
  Serial.print(",");
  Serial.println(adcValue[1]);
}

void setup()
{
  Serial.begin(9600);
  Init();
  Start(snake);
}

void loop()
{
  TASK(INO_BUNDLE, InoBundle(), 10);
  TASK(SNAKE_GAME, Update(snake), 250);
  TASK(CALLBACK, Callback(), 500);
}