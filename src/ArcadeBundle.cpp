/*
 *    LedControl.cpp - A library for controling Leds with a MAX7219/MAX7221
 *    Copyright (c) 2007 Eberhard Fahle
 * 
 *    Permission is hereby granted, free of charge, to any person
 *    obtaining a copy of this software and associated documentation
 *    files (the "Software"), to deal in the Software without
 *    restriction, including without limitation the rights to use,
 *    copy, modify, merge, publish, distribute, sublicense, and/or sell
 *    copies of the Software, and to permit persons to whom the
 *    Software is furnished to do so, subject to the following
 *    conditions:
 * 
 *    This permission notice shall be included in all copies or 
 *    substantial portions of the Software.
 * 
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *    OTHER DEALINGS IN THE SOFTWARE.
 */


#include "ArcadeBundle.h"

uint8_t AB_screen[8][8];

void AB_InitScreen(void){
    AB_HAL_Init();
}

void AB_Setup(void){
    randomSeed(analogRead(0));
    KB_Setup();
    AB_InitScreen();
}

void AB_SetRow(uint8_t y, uint8_t value) {
    for(int x=0; x < 8; x++){
        if((value >> x) & 1U){
            AB_screen[y][x] = LED_ON;
        }else{
            AB_screen[y][x] = LED_OFF;
        }
    }
}

void AB_SetLed(uint8_t x, uint8_t y, uint8_t brightness) {
	AB_screen[y][x] = brightness;
}

uint8_t AB_GetLed(uint8_t x, uint8_t y){
	return AB_screen[y][x];
}

void AB_ClearDisplay(void){
    for(uint8_t y = 0; y<8;y++){
        for(uint8_t x = 0; x < 8; x++){
           AB_screen[x][y] = LED_OFF;
        }
    }
}

bool AB_ClearAnimation(void){
    bool isScreenEmpty = true;

    for(int8_t x = SCREEN_WIDTH-1; x >=0; x--){
        for(int8_t y = 0; y<SCREEN_HEIGHT;y++){    
            if(AB_screen[x][y] == LED_ON){
                
                if(x < (SCREEN_WIDTH-1)){
                    AB_screen[x+1][y] = AB_screen[x][y]; 
                }
                AB_screen[x][y] = LED_OFF;
                isScreenEmpty = false;
                return isScreenEmpty;
            }
        }
    }

    return isScreenEmpty;
}

void AB_UpdateScreen(void){
    AB_HAL_UpdateScreen();
}