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

/*
 * Retrieves the number of milliseconds that have elapsed since
 * the system was started, up to 49.7 days.
 */
uint32_t millis(void) {

	return (uint32_t)GetTickCount();
}

