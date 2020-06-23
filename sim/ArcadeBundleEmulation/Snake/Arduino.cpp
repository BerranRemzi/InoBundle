#include "Arduino.h"
//#include "DualFunctionButton.h"
#include "LedControl.h"

#include "Snake.h"
Snake snake;

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

/* we always wait a bit between updates of the display */
unsigned long delaytime = 100;

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


void rows() {
	for (int row = 0; row < 8; row++) {
		delay(delaytime);
		lc.setRow(0, row, 0b10100000);
		delay(delaytime);
		lc.setRow(0, row, (byte)0);
		for (int i = 0; i < row; i++) {
			delay(delaytime);
			lc.setRow(0, row, 0b10100000);
			delay(delaytime);
			lc.setRow(0, row, (byte)0);
		}
	}
}


void loop() {
	//printf("Hello");
	//Serial.println("Hello");
	//delay(100);
	if (Up.shortPress()) {
		snake.moveSnake(0);
		//Serial.println("Up");
		//lc.setLed(0, 0, 0, true);
	}
	if (Down.shortPress()) {
		snake.moveSnake(1);
		//Serial.println("Down");
		//lc.setLed(0, 1, 0, true);
	}
	if (Right.shortPress()) {
		snake.moveSnake(2);
		//Serial.println("Right");
		//lc.setLed(0, 2, 0, true);
	}
	if (Left.shortPress()) {
		snake.moveSnake(3);
		//Serial.println("Left");
		//lc.setLed(0, 3, 0, true);
	}
	//rows();

	// Print snake head
	lc.setLed(0, (int)snake.head[1], (int)snake.head[0], true);

	// Print food
	lc.setLed(0, (int)snake.food[1], (int)snake.food[0], true);

	// Print snake body
	for (int i{ 0 }; i < 64; ++i) {
		lc.setLed(0, (int)snake.y[i], (int)snake.x[i], true);
	}

	delay(33);	// 30 fps
}