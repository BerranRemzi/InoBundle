#include "graphics.h"

#include <iostream>
#include <math.h>
#include <stdlib.h>                     // Provides exit
#include <ctype.h>                      // Provides toupper
#include "Emulator.h"
#include "Arduino.h"

using namespace std;

const double MATRIX_SIZE = 8;
const double DOT_DIAMETER = 6;
const double DOT_DISTANCE = 7.62;
const double SCREEN_SIZE = 60.2 * 1.05;
const double DOT_OFFSET = (SCREEN_SIZE - (DOT_DISTANCE * (MATRIX_SIZE - 1))) / 2.0;
double multiplier = 5.0;

void draw_dot(int x, int y, int color);

int APIENTRY WinMain
(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    initwindow((int)(SCREEN_SIZE * multiplier), (int)(SCREEN_SIZE * multiplier));

    

    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            draw_dot(x, y, RED);
        }
    }
    setup();
    // Infinite loop to keep program running... only quits when user
    // presses the ESC key to exit.
    while (GetAsyncKeyState(VK_ESCAPE) == false) {
        loop();
    }

}

void draw_dot(int _x, int _y, int _color) {
    double offset = DOT_OFFSET * multiplier;

    int x = int(offset + ((_x * DOT_DISTANCE) * multiplier));

    int y = int(offset + ((_y * DOT_DISTANCE) * multiplier));

    int size = int(DOT_DIAMETER * multiplier / 2.0);

    setcolor(_color);
    setfillstyle(SOLID_FILL, _color);
    fillellipse(y, x, size, size);
}

void updateScreen(uint8_t _input[][8], uint8_t _size) {
    for (int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            if(_input[x][y]==0x00){
                draw_dot(x, y, BLACK);
            }
            else {
                draw_dot(x, y, WHITE);
            }         
        }
    }
}
