#include "Brick.h"

bool checkBit(uint8_t _data, uint8_t _pos){
	return (_data >> _pos) & 1;
}

Brick::Brick() {
	reset();
}

void Brick::reset() {
	for (int i{ 0 }; i < 8; ++i)
		screen[i] = 0;

	brickHeight = SCREEN_HEIGHT - 1;
	screen[brickHeight] = BRICK_DEFAULT;

	totalTicks = TICK_MEDIUM;	// medium level 250ms
}


void Brick::update() {
	static uint8_t cycle = totalTicks;
	if (cycle < totalTicks) {
		cycle++;
		return;
	}
	cycle = 0;

	direction = KB_GetLastDirection();
	MoveBrick();
	PlaceBrick();

	render();
}

void Brick::render() {
	// Delete last part of the brick
	if (direction == DIR_LEFT) {
		for (int i{ SCREEN_HEIGHT - 1 }; i > -1; --i)
			if (checkBit(screen[brickHeight], i)) {
				AB_SetLed(i, brickHeight, LED_OFF);
			}
			screen[brickHeight] <<= 1;
	}
	else if (direction == DIR_RIGHT) {
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
}


void Brick::MoveBrick() {
	//if (direction == DIR_LEFT)
	//if (direction == DIR_RIGHT)
}

void Brick::PlaceBrick() {
	if (direction == DIR_DOWN) {
		++brickHeight;
		screen[brickHeight] = BRICK_DEFAULT;
	}
}
