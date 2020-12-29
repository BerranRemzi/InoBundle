#pragma once
#include <Windows.h>
#include <random>
#include <ctime>
#include "graphics.h"
#include "Utility.h"

#define PROGMEM
#define pgm_read_byte(x) *(x)

void refreshScreen(uint8_t _input[][8], uint8_t _size);
int random(int _max);
void randomSeed(int16_t _input);
int16_t analogRead(int8_t _pin);
bool digitalRead(int16_t _key);
void pinMode(int16_t _pin, int16_t _mode);


extern "C" {
    uint32_t millis(void);
}

void AB_HAL_AnalogPrescaler(uint8_t _prescaler);

enum {
    A0,
    A1,
    A2,
    A3,
    A4,
    A5,
    A6,
    A7
};