#include "Arduino.h"
#include "DualFunctionButton.h"
#include "ArcadeBundle.h"
#include "xOS.h"
#include "Snake.h"
#include "Keyboard.h"

#if ARDUINO

#define VK_UP		5
#define VK_DOWN		2
#define VK_RIGHT	3
#define VK_LEFT		4

#endif

void Task_Keyboard(void);
void Task_Snake(void);
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
Snake snake;

/* we always wait a bit between updates of the display */
unsigned long delaytime = 100;

byte KeyboardByte(void);

void setup() {
	AB_Setup();

	xInit(TaskStruct);      //Struct with function parameters
	
	xTaskCreate(&Task_Keyboard, 10);
	xTaskCreate(&Task_Snake, 10);
	//xTaskCreate(&Task_Brick, 150);
	//xTaskCreate(&Task_Invader, 150);
	//xTaskCreate(&Task_Tetris, 150);
	//xTaskCreate(&Task_Square, 150);
	xTaskCreate(&Task_Screen, 10);
}

void loop() {	
	xLoop();  //xOS task
}

void Task_Snake(void){
	// Game logic
	snake.update();
}
//extern uint8_t AB_screen[8][8];
void Task_Screen(void){
	//lc.updateScreen(&AB_screen[0], 64);
	AB_UpdateScreen();
}

void Task_Keyboard(void){
	KB_ReadAll();
}
