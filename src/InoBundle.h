#include "Arduino.h"

#define UPDATE_PERIOD_US 1000
#define SCREEN_HEIGHT 8
#define SCREEN_WIDTH 8

#define LED_1088

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
void UpdateScreen(void);
void Pixel(int x, int y, bool value);
void WriteRow(int y, uint16_t value);
void WriteCol(int x, uint16_t value);
void Clear(void);