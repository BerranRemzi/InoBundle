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

#define TICK_DELAY 5

uint32_t millis(void) {
    static uint32_t time = 0;
    time += TICK_DELAY;
    Sleep(TICK_DELAY);
    return time;
}

