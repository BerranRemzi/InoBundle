#include "Emulator.h"

int random(int _max) {
	return (rand() % _max);
}
void randomSeed(int16_t _input) {
	srand((unsigned int)time(nullptr));
}

int16_t analogRead(int8_t _pin) {
	return 0;
}
bool digitalRead(int16_t _key) {
	return !GetAsyncKeyState(_key);
}
void pinMode(int16_t _pin, int16_t _mode) {

}

uint8_t AB_screen[8][8];

void AB_Setup() {}
void AB_SetRow(uint8_t y, uint8_t value) {
	for (int x = 0; x < 8; x++) {
		if ((value >> x) & 1U) {
			AB_screen[y][x] = LED_ON;
		}
		else {
			AB_screen[y][x] = LED_OFF;
		}
	}
}

void AB_SetLed(uint8_t x, uint8_t y, uint8_t brightness) {
	AB_screen[y][x] = brightness;
}

uint8_t AB_GetLed(uint8_t x, uint8_t y) {
	return AB_screen[y][x];
}

void AB_UpdateScreen() {
	refreshScreen(AB_screen, 64);
}

bool AB_ClearAnimation() {
	bool isFinished = true;

	for (uint8_t y = 7; y > 0; --y) {
		for (uint8_t x = 0; x < 8; ++x) {
			AB_screen[y][x] = AB_screen[y - 1][x];
			AB_screen[y - 1][x] = LED_OFF;
		}
	}

	for (uint8_t i = 0; i < 8; ++i) {
		for (uint8_t j = 0; j < 8; ++j) {
			if (AB_screen[i][j])
				isFinished = false;
		}
	}

	return isFinished;
}

void AB_ClearDisplay() {
	for (uint8_t y = 0; y < 8; y++) {
		for (uint8_t x = 0; x < 8; x++) {
			AB_screen[x][y] = LED_OFF;
		}
	}
}

/*
 * Retrieves the number of milliseconds that have elapsed since
 * the system was started, up to 49.7 days.
 */
uint32_t millis(void) {

	return (uint32_t)GetTickCount();
}

