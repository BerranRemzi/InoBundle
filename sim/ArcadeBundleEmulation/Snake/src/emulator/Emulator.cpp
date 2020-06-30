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

/* 
 * Retrieves the number of milliseconds that have elapsed since
 * the system was started, up to 49.7 days. 
 */
uint32_t millis(void) {

    return (uint32_t)GetTickCount();
}

