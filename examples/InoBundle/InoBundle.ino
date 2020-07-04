#include "Arduino.h"

//#include "DualFunctionButton.h"
#include "ArcadeBundle.h"
#include "Snake.h"
#include "Brick.h"
#include "Demo.h"

#include "Keyboard.h"
#include "xOS.h"

enum Game_t{
	GAME_SNAKE,
	GAME_BRICK,
	//GAME_INVADER, //not implemented yet
	//GAME_TETRIS,  //not implemented yet
	//GAME_SQUARE,  //not implemented yet
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

uint8_t game = GAME_BRICK;

void setup() {
	AB_Setup();

	xInit(TaskStruct); 	//Struct with function parameters
	
	xTaskCreate(&Task_Keyboard, 10);
	xTaskCreate(&Task_Game, 10);
	xTaskCreate(&Task_Screen, 10);
}

void loop() {	
	xLoop();  //xOS task
}

void Task_Game(void){
	// Game logic
	switch(game){
		case GAME_SNAKE: snake.update(); break;
		case GAME_BRICK: brick.update(); break;
		//case GAME_INVADER: invader.update(); break; 	//not implemented yet
		//case GAME_TETRIS: tetris.update(); break; 	//not implemented yet
		//case GAME_SQUARE: square.update(); break; 	//not implemented yet
		case GAME_DEMO: Demo();/*demo.update();*/ break; // todo: convert to class
		default: 
			/* on invalid number should start first game */
			game = GAME_SNAKE; 
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
		AB_clearDisplay();
		game++;
	}
}