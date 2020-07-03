#include "Brick.h"

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

	//if (KB_IsPressed(VK_DOWN))
	//	direction = DIR_DOWN;
	//if (KB_IsPressed(VK_LEFT))
	//	direction = DIR_LEFT;
	//if (KB_IsPressed(VK_RIGHT))
	//	direction = DIR_RIGHT;
	if (GetAsyncKeyState(VK_LEFT))
		direction = DIR_LEFT;
	if (GetAsyncKeyState(VK_RIGHT))
		direction = DIR_RIGHT;
	if (GetAsyncKeyState(VK_DOWN))
		direction = DIR_DOWN;

	MoveBrick();
	PlaceBrick();

	render();
}

void Brick::render() {
	// Delete last part of the brick
	if (direction == DIR_LEFT) {
		for (int i{ SCREEN_HEIGHT - 1 }; i > -1; --i)
			//if (checkBit(screen[brickHeight], i)) {
			//	setLed(i, brickHeight, LED_OFF);
			//}
			screen[brickHeight] <<= 1;
	}
	else if (direction == DIR_RIGHT) {
		for (int i{ 0 }; i < SCREEN_HEIGHT; ++i)
			//if (checkBit(screen[brickHeight], i)) {
			//	setLed(SCREEN_HEIGHT - 1 - i, brickHeight, LED_OFF);
			//}
			screen[brickHeight] >>= 1;
	}

	// Print brick
	for (int i{ 0 }; i < SCREEN_HEIGHT; ++i) {
		for (int j{ 0 }; j < SCREEN_HEIGHT; ++j) {
			//if (checkBit(screen[i], j)) {
			//	setLed(SCREEN_HEIGHT - 1 - j, i, LED_ON);
			//}
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
