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

void Init(void);
void Task(void);
void UpdateScreen(void);
void Pixel(int x, int y, bool value);
void WriteRow(int y, uint16_t value);
void WriteCol(int x, uint16_t value);
void Clear(void);
boolean GetButtonDown(Button_t input);
boolean GetButton(Button_t input);
void ReadButtons(void);

#endif /* INO_BUNDLE_H */