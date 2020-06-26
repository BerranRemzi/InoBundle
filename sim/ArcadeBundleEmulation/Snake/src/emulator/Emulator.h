#pragma once
#include <Windows.h>
#include <ctime>
#include "graphics.h"

void refreshScreen(uint8_t _input[][8], uint8_t _size);
int random(int _max);
void randomSeed(int16_t _input);
int16_t analogRead(int8_t _pin);
bool digitalRead(int16_t _key);
void pinMode(int16_t _pin, int16_t _mode);