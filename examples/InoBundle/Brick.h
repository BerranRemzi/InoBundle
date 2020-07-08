#pragma once

#include "Arduino.h"
#include "Utility.h"
#include "Keyboard.h"
#include "ArcadeBundle.h"

#define BRICK_DEFAULT 0B11100000

class Brick {
private:
	GameState state = GameState::GAME_RUN;
	uint8_t screen[8];
	int8_t brickHeight{ SCREEN_HEIGHT - 1 };
	Direction_t direction{ Direction_t::RIGHT };
	uint8_t totalTicks = TICK_FAST;
	Position_t pos;
	uint8_t brickDefaultLenght = 4;
	uint8_t brickOnScreenLength = 0;
public:
	Brick();

	void reset();
	void startNewGame();
	void update();
	void render();
	bool isOnScreen();
	bool CollisionDetection();
	bool isReady();
	void MoveBrick();
	void PlaceBrick();
};