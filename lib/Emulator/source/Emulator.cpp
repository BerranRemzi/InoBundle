#include "Emulator.h"
#include <graphics.h>
#include <iostream>
#include <stdlib.h>
#include "Arduino.h"

int height;
int width;
uint16_t *currentBuffer;
uint16_t prevBuffer[16];

extern uint16_t * GetScreenBuffer(int *width, int *height);
extern uint16_t screen[];

int APIENTRY WinMain(
  HINSTANCE hInstance,
  HINSTANCE hPrevInstance,
  LPSTR     lpCmdLine,
  int       nShowCmd
)
{
   initwindow((int)(SCREEN_SIZE * multiplier), (int)(SCREEN_SIZE * multiplier));
   
   currentBuffer = GetScreenBuffer(&width, &height);

   setup();

   if(false == Serial.isOpen()){
      FreeConsole();
   }
   while(!kbhit()){
      incrementMillis(10);
      loop();
      refreshScreen();
      delay(10);
   }
       
   closegraph();
   return 0;
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

void refreshScreen() {
   
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            if ((screen[y]>>x & 1U) != (prevBuffer[y]>>x & 1U)) {
                if ((screen[y]>>x & 1U) == 0x00) {
                    draw_dot(x, y, BLACK);
                }
                else {
                    draw_dot(x, y, WHITE);
                }
            }
        }
        prevBuffer[y] = screen[y];
    }
}