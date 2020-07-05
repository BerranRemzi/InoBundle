#pragma once
#include <Windows.h>
#include <ctime>
#include "graphics.h"
#include "Utility.h"


void refreshScreen(uint8_t _input[][8], uint8_t _size);
int random(int _max);
void randomSeed(int16_t _input);
int16_t analogRead(int8_t _pin);
bool digitalRead(int16_t _key);
void pinMode(int16_t _pin, int16_t _mode);

void AB_SetLed(uint8_t, uint8_t, uint8_t);
void AB_Setup();
void AB_UpdateScreen();
void AB_SetRow(uint8_t, uint8_t);
bool AB_ClearAnimation();
void AB_ClearDisplay();

extern "C" {
	uint32_t millis(void);
}