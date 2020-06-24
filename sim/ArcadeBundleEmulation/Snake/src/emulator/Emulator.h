#pragma once
#include <Windows.h>
#include <ctime>
#include "graphics.h"

void updateScreen(uint8_t _input[][8], uint8_t _size);
int random(int _max);
void randomSeed(int16_t _input);
int16_t analogRead(int8_t _pin);