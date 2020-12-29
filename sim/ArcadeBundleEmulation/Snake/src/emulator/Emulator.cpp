#include "Emulator.h"

int random(int _max) {
    return (rand() % _max);
}
void randomSeed(int16_t) {
    srand((unsigned int)time(nullptr));
}

int16_t analogRead(int8_t) {
    return 0;
}
bool digitalRead(int16_t _key) {
    return !GetAsyncKeyState(_key);
}
void pinMode(int16_t, int16_t) {
    /* Just for emulation on desktop*/
}

extern uint8_t AB_screen[8][8];

void AB_HAL_UpdateScreen() {
    refreshScreen(AB_screen, 64);
}

/*
 * Retrieves the number of milliseconds that have elapsed since
 * the system was started, up to 49.7 days.
 */
uint32_t millis(void) {
    delay(10);
    return (uint32_t)GetTickCount64();
}

void AB_HAL_AnalogPrescaler(uint8_t _prescaler) {

}