#include "Arduino.h"

#include "DualFunctionButton.h"
#include "ArcadeBundle.h"
#include "Brick.h"

#include "Keyboard.h"
#include "xOS.h"

void Task_Keyboard(void);
void Task_Brick(void);
void Task_Screen(void);
void Task_Debug(void);

Task_t TaskStruct[4];

#if 0
DualFunctionButton Up(VK_UP, 500, INPUT_PULLUP);
DualFunctionButton Down(VK_DOWN, 500, INPUT_PULLUP);
DualFunctionButton Right(VK_RIGHT, 500, INPUT_PULLUP);
DualFunctionButton Left(VK_LEFT, 500, INPUT_PULLUP);
#endif
/*
 Now we need a LedControl to work with.
 ***** These pin numbers will probably not work with your hardware *****
 pin 12 is connected to the DataIn
 pin 11 is connected to the CLK
 pin 10 is connected to LOAD
 We have only a single MAX72XX.
 */

Brick brick;

void setup() {
	AB_Setup();

	xInit(TaskStruct);      //Struct with function parameters
	
	xTaskCreate(&Task_Keyboard, 10);
	//xTaskCreate(&Task_Snake, 10);
	xTaskCreate(&Task_Brick, 10);
	//xTaskCreate(&Task_Invader, 150);
	//xTaskCreate(&Task_Tetris, 150);
	//xTaskCreate(&Task_Square, 150);
	xTaskCreate(&Task_Screen, 10);
	xTaskCreate(&Task_Debug, 500);
}

void loop() {	
	//AB_UpdateScreen();
	xLoop();  //xOS task
}

void Task_Brick(void){
	// Game logic
	brick.update();
}

void Task_Screen(void){
	AB_UpdateScreen();
}

void Task_Keyboard(void){
	KB_ReadAll();
}

void Task_Debug(void){
	static uint8_t state = LED_OFF;
	state = ~state;
	AB_SetLed(0, 0, state);
}