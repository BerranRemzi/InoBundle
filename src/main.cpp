#include "Arduino.h"
#include "InoBundle.h"
#include "games/Tetris.h"
#include "games/Demo.h"

void setup()
{
  Init();
  Init_Tetris();
}

void loop()
{
  Task_Tetris();
  //Task_Demo();
  Task();
}