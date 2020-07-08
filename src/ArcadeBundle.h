/*
 *    LedControl.h - A library for controling Leds with a MAX7219/MAX7221
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

#ifndef LedControl_h
#define LedControl_h

#include "ArcadeBundle_Types.h"
#include "Keyboard.h"

#ifdef ARDUINO

#include <avr/pgmspace.h>
#include "ArcadeBundle_HAL.h"

#else

#define AB_HAL_Init()

extern void AB_HAL_UpdateScreen(void);

#endif

#define SPI_CS_PIN  10

//void AB_InitPins(void);
void AB_InitScreen(void);
void AB_Setup(void);

void AB_SetRow(uint8_t y, uint8_t value);
void AB_SetLed(uint8_t x, uint8_t y, uint8_t brightness);
uint8_t AB_GetLed(uint8_t x, uint8_t y);
void AB_UpdateScreen(void);

//void AB_DisplayTest(void);
//void AB_SetScanLimit(uint8_t _limit);
//void AB_DecodeMode(void);

//void AB_Shutdown(bool);
//void AB_HAL_SetIntensity(uint8_t);
void AB_ClearDisplay(void);
//void AB_SpiTransfer(volatile byte opcode, volatile byte data);
bool AB_ClearAnimation(void);

#endif	//LedControl.h
