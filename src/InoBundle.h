#ifndef INO_BUNDLE_H
#define INO_BUNDLE_H

#include "Arduino.h"
#include "InoBundle_Cfg.h"

#ifdef LED_1088
#define ROW_ON LOW
#define ROW_OFF HIGH
#define COLUMN_ON LOW
#else
#define ROW_ON LOW
#define ROW_OFF HIGH
#define COLUMN_ON LOW
#endif

#define TASK(NAME, FUNCTION, PERIOD)                     \
  static uint32_t millis##NAME = 0;                      \
  if ((unsigned long)(millis() - millis##NAME) > PERIOD) \
  {                                                      \
    millis##NAME = millis();                             \
    FUNCTION;                                            \
  }

void Init(void);
void Task(void);
void UpdateScreen(void);
void Pixel(int x, int y, bool value);
boolean GetPixel(int x, int y);
void WriteRow(int y, uint16_t value);
void WriteCol(int x, uint16_t value);
void Clear(void);
boolean GetButtonDown(Button_t input);
boolean GetButton(Button_t input);
void ReadButtons(void);
void ComputeButtonParameter(Button_t input, uint8_t * value, uint8_t * btnGroup);

#endif /* INO_BUNDLE_H */