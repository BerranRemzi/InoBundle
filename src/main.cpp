#include "Arduino.h"
#include "InoBundle.h"
#include "games/Tetris.h"
#include "games/Demo.h"

void setup()
{
  Init();
}

void loop()
{
  //Task_Tetris();
  Task_Demo();
}