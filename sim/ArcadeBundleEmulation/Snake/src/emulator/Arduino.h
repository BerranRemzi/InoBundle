#pragma once
#include <stdio.h>
#include <stdint.h>
#include <Windows.h>
#include "Emulator.h"

#define OUTPUT 0
#define INPUT 1
#define INPUT_PULLUP 2

void setup(void);
void loop(void);

class SoftwareSerial {
public:
    void begin(uint32_t _baud) {
    }
    void print(char const* const _input) {
        printf("%s", _input);
    }
    void println(char const* const _input) {
        print(_input);
        print("\n");
    }
};

static SoftwareSerial Serial;
