#pragma once
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdint.h>


void setup(void);
void loop(void);
void delay(uint32_t _delay);

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