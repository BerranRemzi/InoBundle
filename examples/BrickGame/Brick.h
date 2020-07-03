#pragma once

#include "Utility.h"
#include "Arduino.h"

#define BRICK_DEFAULT 0b1110'0000

class Brick {
public:
	uint8_t screen[8];
	uint8_t brickHeight{ SCREEN_HEIGHT - 1 };
	Direction_t direction{ DIR_RIGHT };
	uint8_t totalTicks;
public:
	Brick();

	void reset();

	void update();
	void render();

	void MoveBrick();
	void PlaceBrick();
};