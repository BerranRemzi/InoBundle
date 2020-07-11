#include "Brick.h"

Brick::Brick() {
	reset();
}

void Brick::reset() {
	for (int i{ 0 }; i < 8; ++i)
		screen[i] = 0;

	brickHeight = SCREEN_HEIGHT - 1;
	totalTicks = TICK_MEDIUM;	// medium level 250ms
	bricksOnScreen = 0;
	direction = Direction_t::RIGHT;
	brickLenght = 3;
}
void Brick::startNewGame() {
	bricksOnScreen = 0;
}

bool Brick::isReady() {
	bool returnValue = false;

	static uint8_t cycle = totalTicks;
	if (cycle < totalTicks) {
		cycle++;
		returnValue = false;
	}
	else {
		cycle = 0;
		returnValue = true;
	}
	return returnValue;
}

void Brick::update() {
	bool ready = isReady();
	bool isDetected = false;

	switch (state) {
	case GameState::PAUSE:
		break;
	case GameState::GAME_RUN:
		totalTicks = TICK_MEDIUM;
		state = GameState::GAME_WAIT;

		if ((KB_IsKeyToggled(VK_DOWN) && KB_IsKeyDown(VK_DOWN))
			|| (KB_IsKeyToggled(VK_UP) && KB_IsKeyDown(VK_UP))) {
			isDetected = true;
		}

		/* Place brick based on gameplay state */
		if (BrickState::GOTO_RESTART == PlaceBrick(isDetected)) {
			state = GameState::ANIM_RUN;
		}
		/* Move bricks to right and left directions */
		MoveBrick();

		render();
		break;
	case GameState::GAME_WAIT:
		if (ready)
			state = GameState::GAME_RUN;
		break;
	case GameState::ANIM_RUN:
		totalTicks = 2;
		state = GameState::ANIM_WAIT;

		ready = AB_ClearAnimation();
		if (ready) {
			state = GameState::RESTART;
		}
		break;
	case GameState::ANIM_WAIT:
		if (ready)
			state = GameState::ANIM_RUN;
		break;
	case GameState::BLINK:
		break;
	case GameState::RESTART:
		reset();
		render();
		state = GameState::GAME_RUN;
		break;
	}
}

BrickState Brick::PlaceBrick(bool _collision) {
	switch (brickState) {
	case BrickState::MOVING:
		if (_collision) {
			brickState = BrickState::CHECK_NEW_LINE;
		}
		break;
	case BrickState::CHECK_NEW_LINE:

		if (brickHeight >= 0) {

			if (brickHeight < 7) {
				/* trim brick based on previous row */
				screen[brickHeight] &= screen[brickHeight + 1];
				/* get lenght of brick after trimming */
				brickLenght = GetBrickLength(brickHeight);
			}

			/* switch to next row */
			MoveToNextRow();

			/* change brick direction to opposite side */
			GotoOppositeDirection();
		}

		if (brickHeight == OUT_OF_SCREEN || brickLenght == 0) {
			brickState = BrickState::GOTO_RESTART;
		}
		else {
			brickState = BrickState::MOVING;
		}

		break;
	case BrickState::GOTO_RESTART:
		brickState = BrickState::MOVING;
		break;
	default:
		break;
	}
	return brickState;
}

void Brick::MoveToNextRow() {
	if (brickHeight > OUT_OF_SCREEN) {
		brickHeight--;
	}
	bricksOnScreen = 0;
}

void Brick::GotoOppositeDirection() {
	if (direction == Direction_t::RIGHT) {
		direction = Direction_t::LEFT;
	}
	else {
		direction = Direction_t::RIGHT;
	}
}

uint8_t Brick::GetBrickLength(uint8_t _row) {
	uint8_t counter = 0;
	for (uint8_t i = 0; i < 8; i++) {

		if (checkBit(screen[brickHeight], i) == true) {
			counter++;
		}
	}

	// If brick is at top, the next brick length must be 0
	if (_row == 0) {
		counter = 0;
	}
	return counter;
}

void Brick::render() {
	for (uint8_t y = 0; y < 8; y++) {
		for (uint8_t x = 0; x < 8; x++) {
			if (checkBit(screen[y], x)) {
				AB_SetLed(x, y, LED_ON);
			}
			else {
				AB_SetLed(x, y, LED_OFF);
			}
		}
	}
}

bool  Brick::isOnScreen() {
	return (bricksOnScreen > 0);
}

void Brick::MoveBrick() {

	if (direction == Direction_t::RIGHT) {
		screen[brickHeight] >>= 1;
		if (brickLenght - bricksOnScreen > 0) {
			screen[brickHeight] |= 0x80;
			bricksOnScreen++;
		}
		if (screen[brickHeight] == 0x00) {
			direction = Direction_t::LEFT;
			bricksOnScreen = 0;
		}
	}
	else if (direction == Direction_t::LEFT) {
		screen[brickHeight] <<= 1;
		if (brickLenght - bricksOnScreen > 0) {
			screen[brickHeight] |= 0x01;
			bricksOnScreen++;
		}
		if (screen[brickHeight] == 0x00) {
			direction = Direction_t::RIGHT;
			bricksOnScreen = 0;
		}
	}
}

