#ifndef EMULATOR_H
#define EMULATOR_H

#include <stdint.h>

const double MATRIX_SIZE = 8;
const double DOT_DIAMETER = 6;
const double DOT_DISTANCE = 7.62;
const double SCREEN_SIZE = 60.2 * 1.05;
const double DOT_OFFSET = (SCREEN_SIZE - (DOT_DISTANCE * (MATRIX_SIZE - 1))) / 2.0;
double multiplier = 5.0;
void draw_dot(int _x, int _y, int _color);
void refreshScreen();

#endif /* EMULATOR_H */