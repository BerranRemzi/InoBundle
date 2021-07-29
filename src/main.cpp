#include "Arduino.h"
#include "InoBundle.h"
#include "games/Tetris.h"
#include "games/Demo.h"
#include "games/Snake.h"

Snake_t* snake;

Snake_t tempSnake;
void setup()
{
  snake = &tempSnake;
  Init();
  //Init_Tetris();
  Start(snake);
}
uint32_t previousUpdateMillis = 0;
void loop()
{
  //Task_Tetris();
  //Task_Demo();
  Task();
  if ((unsigned long)(millis() - previousUpdateMillis) > 300)
    {
        previousUpdateMillis = millis();
        Update(snake);
    }
}