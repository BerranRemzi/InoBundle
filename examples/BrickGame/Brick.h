#pragma once

#include "Arduino.h"
#include "Utility.h"
#include "Keyboard.h"
#include "ArcadeBundle.h"

#define BRICK_DEFAULT 0B11100000

class Brick {
public:
	uint8_t screen[8];
	uint8_t brickHeight{ SCREEN_HEIGHT - 1 };
	Direction_t direction{ DIR_RIGHT };
	uint8_t totalTicks;
	Position_t pos;
	const uint8_t brickDefaultLenght = 3; 
	uint8_t brickOnScreenLength = 0;
public:
	Brick();

	void reset();

	void update();
	void render();
	bool isOnScreen();
	void CollisionDetection();
	void MoveBrick();
	void PlaceBrick();
};