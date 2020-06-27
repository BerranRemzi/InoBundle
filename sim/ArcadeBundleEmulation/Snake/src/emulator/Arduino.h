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


void setLed(int x, int y, uint8_t brightness);
//uint32_t millis();

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

class DualFunctionButton {
public:
	DualFunctionButton(int _buttonPin, long _longPressDebounceDelay, char _inputMode) {
		buttonPin = _buttonPin;
	};
	bool shortPress() {
		return GetAsyncKeyState(buttonPin);
	}
	bool longPress() {
		return 0;
	}
	int buttonPin = -1;
};

static SoftwareSerial Serial;

