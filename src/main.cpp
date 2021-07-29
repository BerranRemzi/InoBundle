#include "Arduino.h"
#include "InoBundle.h"
#include "games/Tetris.h"
#include "games/Demo.h"
#include "games/Snake.h"

Snake_t* snake;

void setup()
{
  Init();
  //Init_Tetris();
  Start(snake);
}

void loop()
{
  //Task_Tetris();
  //Task_Demo();
  Task();
  Update(snake);
}