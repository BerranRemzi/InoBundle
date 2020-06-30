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
	Position_t body[SCREEN_WIDTH * SCREEN_HEIGHT];
	Position_t tail;
	Position_t currentHead;
	Position_t previousHead;
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
