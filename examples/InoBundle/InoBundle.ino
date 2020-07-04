#include "Arduino.h"

//#include "DualFunctionButton.h"
#include "ArcadeBundle.h"
#include "Snake.h"
#include "Brick.h"
#include "Demo.h"

#include "Keyboard.h"
#include "xOS.h"

enum {
	GAME_SNAKE,
	GAME_BRICK,
	GAME_INVADER,
	GAME_TETRIS,
	GAME_SQUARE,
	GAME_DEMO,
	GAME_COUNT
};

void Task_Keyboard(void);
void Task_Game(void);
void Task_Screen(void);

Task_t TaskStruct[4];

Snake snake;
Brick brick;
//Invader invader;		//not implemented yet
//Tetris tetris;		//not implemented yet
//Square square;		//not implemented yet
//Demo demo;			//not implemented yet

uint8_t game = GAME_DEMO;

void setup() {
	AB_Setup();

	xInit(TaskStruct);      //Struct with function parameters
	
	xTaskCreate(&Task_Keyboard, 10);
	xTaskCreate(&Task_Game, 10);
	xTaskCreate(&Task_Screen, 10);
}

void loop() {	
	//AB_UpdateScreen();
	xLoop();  //xOS task
}

void Task_Game(void){
	// Game logic
	switch(game){
		case GAME_SNAKE: snake.update(); break;
		case GAME_BRICK: brick.update(); break;
		//case GAME_INVADER: invader.update(); break;
		//case GAME_TETRIS: tetris.update(); break;
		//case GAME_SQUARE: square.update(); break;
		case GAME_DEMO: Demo();/*demo.update();*/ break; // todo: convert to class
		default: 
			game = GAME_BRICK;
		break;
	}
}

void Task_Screen(void){
	AB_UpdateScreen();
}

void Task_Keyboard(void){
	KB_ReadAll();
	/* wait for 100ticks = 1000ms */
	if(KB_IsKeyDownLong(VK_UP, 100)){
		game++;
	}
}