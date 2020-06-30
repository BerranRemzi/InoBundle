#include "Arduino.h"
#include "DualFunctionButton.h"
#include "ArcadeBundle.h"
#include "xOS.h"
#include "Brick.h"
#include "Keyboard.h"

#if ARDUINO

#define VK_UP		5
#define VK_DOWN		2
#define VK_RIGHT	3
#define VK_LEFT		4

#endif

void Task_Keyboard(void);
void Task_Brick(void);
void Task_Screen(void);
Task_t TaskStruct[4];

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
Brick brick;

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

	xInit(TaskStruct);      //Struct with function parameters
	
	xTaskCreate(&Task_Keyboard, 10);
	//xTaskCreate(&Task_Snake, 10);
	xTaskCreate(&Task_Brick, 10);
	//xTaskCreate(&Task_Invader, 150);
	//xTaskCreate(&Task_Tetris, 150);
	//xTaskCreate(&Task_Square, 150);
	xTaskCreate(&Task_Screen, 10);
}

void loop() {	
	xLoop();  //xOS task
}

void Task_Brick(void){
	// Game logic
	brick.update();
}

void Task_Screen(void){
	lc.updateScreen(&screen[0], 64);
}

void Task_Keyboard(void){
	KB_ReadAll();
}
