#include "Emulator.h"

int random(int _max) {
    return (rand() % _max);
}
void randomSeed(int16_t _input) {
    srand(time(nullptr));
}

int16_t analogRead(int8_t _pin) {
    return 0;
}