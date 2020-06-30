#pragma once
#include <stdint.h>
#include "Arduino.h"

#define LED_ON 0xFF
#define LED_OFF 0x00

#define SCREEN_WIDTH 	8
#define SCREEN_HEIGHT 	8

#define OUT_OF_SCREEN 	-1

#define SNAKE_START_SIZE 2

enum Sound_t {
	SILENCE,
	SIZE_UP,
	DEAD,
	MOVE
};

enum Direction_t {
	DIR_STOPPED,
	DIR_UP,
	DIR_DOWN,
	DIR_RIGHT,
	DIR_LEFT
};

enum Axis_t {
	AXIS_X,
	AXIS_Y
};

typedef struct Position_t {
	int8_t x;
	int8_t y;
};

extern void setLed(int x, int y, uint8_t brightness);

class Snake {
public:

	//int8_t x[64];		// Snake body X
	//int8_t y[64];		// Snake body Y
	Position_t body[SCREEN_WIDTH * SCREEN_HEIGHT]; //todo: change [x,y] array with this struct array
	//int8_t head[4];		// 0 - head X, 1 - head Y, 2 - old head X, 3 - old head Y
	Position_t currentHead; //todo: change [x,y] array with this struct array
	Position_t oldHead;
	//int8_t bodyLast[2];	// 0 - last body X, 1 - last body Y
	Position_t  bodyLast;
	//int8_t food[2];		// 0 - food X, 1 - food Y
	Position_t food;

	int8_t size{ 0 };
	Direction_t direction;
	Sound_t sound;
	uint8_t speed{ 0 };
	uint8_t totalTicks;
	bool isFoodGenerated = false;
	Direction_t lastDirection = DIR_STOPPED;
public:
	Snake();
	void setup();
	void newGame(void);
	void reset();

	void render();
	void update();

	void generateFood();
	void putFood();
	void extendSnake();
	bool moveSnake();
	void moveBody();
	bool isInScreen(int i);
	Sound_t playSound();
	Direction_t getLastDirection(void);
};
