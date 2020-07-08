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
	totalTicks = TICK_MEDIUM;	// medium level 250ms
	brickOnScreenLength = 0;
	direction = Direction_t::RIGHT;
	brickDefaultLenght = 3;
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
		totalTicks = TICK_FAST;
		state = GameState::GAME_WAIT;

		if ((KB_IsKeyToggled(VK_DOWN) && KB_IsKeyDown(VK_DOWN))
		||(KB_IsKeyToggled(VK_UP) && KB_IsKeyDown(VK_UP))) {
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
		totalTicks = 2;
		state = GameState::ANIM_WAIT;

		ready = AB_ClearAnimation();
		if (ready) {
			state = GameState::RESTART;
		}
		//render();
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
		//KB_Reset();
		state = GameState::GAME_RUN;
		break;
	}
}

bool Brick::CollisionDetection() {
	bool returnValue = false;



	if (brickHeight > 0) {
		
		if(brickHeight < 7 ){
			/* trim brick based on previous row */
			screen[brickHeight] &= screen[brickHeight+1];

			/* get lenght of brick after trimming */
			brickDefaultLenght = 0;
			for(uint8_t i =0; i < 8; i++){
				if(checkBit(screen[brickHeight+1], i) == true){
					brickDefaultLenght++;
				}
			}
			if(brickDefaultLenght==0){
				brickHeight = 0;
			}
		}
		
		/* switch to next row */
		brickHeight--;

		brickOnScreenLength = 0;

		/* change brick direction to opposite side */
		if(direction == Direction_t::RIGHT){
			direction = Direction_t::LEFT;
		}else{
			direction = Direction_t::RIGHT;
		}

		returnValue = false;
	}
	else {
		reset();
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

}
