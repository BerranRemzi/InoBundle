#include "Arduino.h"
#include "DualFunctionButton.h"
#include "ArcadeBundle.h"

#include "Snake.h"

#if ARDUINO

#define VK_UP		4
#define VK_DOWN		5
#define VK_RIGHT	6
#define VK_LEFT		7

#endif

DualFunctionButton Up(VK_UP, 500, INPUT_PULLUP);
DualFunctionButton Down(VK_DOWN, 500, INPUT_PULLUP);
DualFunctionButton Right(VK_RIGHT, 500, INPUT_PULLUP);
DualFunctionButton Left(VK_LEFT, 500, INPUT_PULLUP);

/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn
 pin 11 is connected to the CLK
 pin 10 is connected to LOAD
 We have only a single MAX72XX.
 */
const int chipSelectPin = 10;
//LedControl lc(12, 11, 10, 1);
LedControl lc(chipSelectPin, 1);
Snake snake;

/* we always wait a bit between updates of the display */
unsigned long delaytime = 100;


static uint8_t screen[8][8];

void setLed(int x, int y, uint8_t brightness) {
	screen[y][x] = brightness;
}

byte KeyboardByte(void);

void setup() {
	/*
	The MAX72XX is in power-saving mode on startup,
	we have to do a wakeup call
	 */
	lc.shutdown(0, false);
	/* Set the brightness to a medium values */
	lc.setIntensity(0, 1); //min = 1, max = 16
	/* and clear the display */
	lc.clearDisplay(0);

	randomSeed(analogRead(0));
  pinMode(VK_UP, INPUT_PULLUP);
  pinMode(VK_DOWN, INPUT_PULLUP);
  pinMode(VK_LEFT, INPUT_PULLUP);
  pinMode(VK_RIGHT, INPUT_PULLUP);
}

void loop() {
	snake.update(KeyboardByte());

	snake.render();

	if (snake.playSound() > 0) {}

	lc.updateScreen(&screen[0], 64);

	delay(snake.loopTime());
}

byte KeyboardByte(void) {
	//byte returnValue = (Up.shortPress() << 0U) | (Down.shortPress() << 1U) | (Right.shortPress() << 2U) | (Left.shortPress() << 3U);
	byte returnValue = (!digitalRead(VK_UP) << 0) |(!digitalRead(VK_DOWN) << 1) |(!digitalRead(VK_RIGHT) << 2) | (!digitalRead(VK_LEFT) << 3) ;
	return returnValue;
}
