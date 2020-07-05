#include "Brick.h"

bool checkBit(uint8_t _data, uint8_t _pos) {
	return ((_data >> _pos) & 1U);
}

Brick::Brick() {
	reset();
}

void Brick::reset() {
	for (int i{ 0 }; i < 8; ++i)
		screen[i] = 0;

	brickHeight = SCREEN_HEIGHT - 1;
	//screen[brickHeight] = BRICK_DEFAULT;

	totalTicks = TICK_MEDIUM;	// medium level 250ms
	brickOnScreenLength = 0;
	direction = Direction_t::RIGHT;
}
void Brick::startNewGame() {
	brickOnScreenLength = 0;
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

	switch (state) {
	case GameState::PAUSE:
		break;
	case GameState::GAME_RUN:
		state = GameState::GAME_WAIT;

		if (KB_IsKeyToggled(VK_DOWN) && KB_IsKeyDown(VK_DOWN)) {

			bool isDetected = CollisionDetection();
			if (isDetected) {
				state = GameState::ANIM_RUN;
				break;
			}
		}
		MoveBrick();
		render();
		break;
	case GameState::GAME_WAIT:
		if (ready)
			state = GameState::GAME_RUN;
		break;
	case GameState::ANIM_RUN:
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
		state = GameState::GAME_RUN;
		break;
	}

}

bool Brick::CollisionDetection() {
	bool returnValue = false;
	if (brickHeight > 0) {
		brickHeight--;
		brickOnScreenLength = 0;
		/*if(direction==Direction_t::RIGHT){
			direction==Direction_t::LEFT;
		}else{
			direction==Direction_t::RIGHT;
		}*/
		returnValue = false;
	}
	else {

		//reset();
		returnValue = true;
	}
	return returnValue;
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
	return (brickOnScreenLength > 0);
}

void Brick::MoveBrick() {

	// Delete last part of the brick
#if 0
	if (direction == Direction_t::LEFT) {
		for (int i{ SCREEN_HEIGHT - 1 }; i > -1; --i)
			if (checkBit(screen[brickHeight], i)) {
				AB_SetLed(i, brickHeight, LED_OFF);
			}
		screen[brickHeight] <<= 1;
	}
	else if (direction == Direction_t::RIGHT) {
		for (int i{ 0 }; i < SCREEN_HEIGHT; ++i)
			if (checkBit(screen[brickHeight], i)) {
				AB_SetLed(SCREEN_HEIGHT - 1 - i, brickHeight, LED_OFF);
			}
		screen[brickHeight] >>= 1;
	}

	// Print brick
	for (int i{ 0 }; i < SCREEN_HEIGHT; ++i) {
		for (int j{ 0 }; j < SCREEN_HEIGHT; ++j) {
			if (checkBit(screen[i], j)) {
				AB_SetLed(SCREEN_HEIGHT - 1 - j, i, LED_ON);
			}
		}
	}
#endif


	if (direction == Direction_t::RIGHT) {
		screen[brickHeight] >>= 1;
		if (brickDefaultLenght - brickOnScreenLength > 0) {
			screen[brickHeight] |= 0x80;
			brickOnScreenLength++;
		}
		if (screen[brickHeight] == 0x00) {
			direction = Direction_t::LEFT;
			brickOnScreenLength = 0;
		}
	}
	else if (direction == Direction_t::LEFT) {
		screen[brickHeight] <<= 1;
		if (brickDefaultLenght - brickOnScreenLength > 0) {
			screen[brickHeight] |= 0x01;
			brickOnScreenLength++;
		}
		if (screen[brickHeight] == 0x00) {
			direction = Direction_t::RIGHT;
			brickOnScreenLength = 0;
		}
	}
}

void Brick::PlaceBrick() {
	if (direction == Direction_t::DOWN) {
		++brickHeight;
		screen[brickHeight] = BRICK_DEFAULT;
	}
}
