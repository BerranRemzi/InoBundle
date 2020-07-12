#pragma once

#include "Game.h"
#include "Arduino.h"
#include "Utility.h"
#include "Keyboard.h"
#include "ArcadeBundle.h"

enum class BrickState {
	MOVING,
	CHECK_NEW_LINE,
	GOTO_RESTART
};

class Brick : public Game {
public:
	Brick();
	void update() override;

private:
	GameState state = GameState::GAME_RUN;
	uint8_t screen[8];
	int8_t brickHeight{ SCREEN_HEIGHT - 1 };
	Direction_t direction{ Direction_t::RIGHT };
	Timer* timer;
	Position_t pos;
	uint8_t brickLenght = 5;
	uint8_t bricksOnScreen = 0;
	BrickState brickState = BrickState::MOVING;

	void reset();
	void render();
	BrickState PlaceBrick(bool _collision);
	void MoveBrick();
	void GotoOppositeDirection();
	uint8_t GetBrickLength(uint8_t _row);
	void MoveToNextRow();
};