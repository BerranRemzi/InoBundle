#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <Windows.h>
#include <chrono>
#include "Arduino.h"

void delay(uint32_t _delay) {
	Sleep(_delay);
}

int main() {
	setup();
	while (GetAsyncKeyState(VK_ESCAPE) == false) {
		loop();
	}

	return 0;
}