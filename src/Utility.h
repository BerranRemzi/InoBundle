#pragma once

//#include <bitset>
#include <stdint.h>
//#include "Keyboard.h"

#define LED_ON			0xFF
#define LED_OFF			0x00

#define SCREEN_WIDTH 	8
#define SCREEN_HEIGHT 	8

#define OUT_OF_SCREEN	-1

enum Tick_t {
	TICK_SLOW = 50,			//slow 		50 x 10 = 500ms
	TICK_MEDIUM = 25,		//medium	25 x 10 = 250ms
	TICK_FAST = 15			//fast		15 x 10 = 150ms
};

enum Sound_t {
	SNAKE_SILENCE,
	SNAKE_SIZE_UP,
	SNAKE_DEAD,
	SNAKE_MOVE
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

static Position_t matrix[SCREEN_WIDTH * SCREEN_HEIGHT];

//extern void setLed(int x, int y, uint8_t brightness);