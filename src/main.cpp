#include "Arduino.h"
#include "InoBundle.h"

void setup()
{
  Init();
  Clear();
}

void loop()
{
  WriteCol(0, 0xFF);
  WriteCol(7, 0xFF);
  WriteRow(0, 0xFF);
  WriteRow(7, 0xFF);
  Pixel(3, 3, HIGH);
  Pixel(3, 4, HIGH);
  Pixel(4, 3, HIGH);
  Pixel(4, 4, HIGH);
  delay(500);

  Clear();
  delay(500);
}