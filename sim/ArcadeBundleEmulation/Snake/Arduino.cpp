#include "Arduino.h"
#include "DualFunctionButton.h"
#include "LedControl.h"

#include "Snake.h"

DualFunctionButton Up(VK_UP, 500, INPUT);
DualFunctionButton Down(VK_DOWN, 500, INPUT);
DualFunctionButton Right(VK_RIGHT, 500, INPUT);
DualFunctionButton Left(VK_LEFT, 500, INPUT);

/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn
 pin 11 is connected to the CLK
 pin 10 is connected to LOAD
 We have only a single MAX72XX.
 */
LedControl lc(12, 11, 10, 1);
Snake snake;

/* we always wait a bit between updates of the display */
unsigned long delaytime = 100;

static uint8_t screen[8][8];

void setLed(int x, int y, uint8_t brightness) {
	screen[y][x] = brightness;
}

void setup() {
	Serial.begin(9600);
	//Serial.println("Snake 8x8");

	/*
	The MAX72XX is in power-saving mode on startup,
	we have to do a wakeup call
	 */
	lc.shutdown(0, false);
	/* Set the brightness to a medium values */
	lc.setIntensity(0, 1); //min = 1, max = 16
	/* and clear the display */
	lc.clearDisplay(0);
}

void loop() {
	if (Up.shortPress()) {
		snake.moveSnake(0);
	}
	if (Down.shortPress()) {
		snake.moveSnake(1);
	}
	if (Right.shortPress()) {
		snake.moveSnake(2);
	}
	if (Left.shortPress()) {
		snake.moveSnake(3);
	}
	//rows();

	snake.update();
	snake.render();
	updateScreen(&screen[0], 64);

	delay(33);	// 30 fps
}