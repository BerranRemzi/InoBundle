#include "Snake.h"

Snake::Snake() {

	for (int i{ 0 }; i < 64; ++i) {
		x[i] = -1;
		y[i] = -1;
	}

	head[0] = random(8);
	head[1] = random(8);

	generateFood();
}

void Snake::render() {
	// Print snake body
	for (int _y{ 0 }; _y < SCREEN_HEIGHT; ++_y) {
		for (int _x{ 0 }; _x < SCREEN_HEIGHT; ++_x) {
			if (isInScreen(_y + _x * (SCREEN_WIDTH - 1))) {
				setLed((int)x[_y + _x * (SCREEN_WIDTH - 1)], (int)y[_y + _x * (SCREEN_WIDTH - 1)], LED_ON);
			}
			else {
				setLed(_x, _y, LED_OFF);
			}
		}
	}

	// Print snake head
	setLed((int)head[0], (int)head[1], LED_ON);

	// Print food
	setLed((int)food[0], (int)food[1], LED_ON);
}

void Snake::update() {

}

void Snake::generateFood() {
	food[0] = random(8);
	food[1] = random(8);
}

void Snake::extendSnake() {

}

void Snake::moveSnake(int direction) {
	switch (direction) {
	case 0:	// Up
		--head[1];
		break;
	case 1:	// Down
		++head[1];
		break;
	case 2:	// Right
		++head[0];
		break;
	case 3: // Left
		--head[0];
		break;
	default:
		break;
	}
}

bool Snake::isInScreen(int i) {
	bool value{ true };
	if ((x[i] < 0 || x[i] >= SCREEN_WIDTH) &&
		(y[i] < 0 || y[i] >= SCREEN_HEIGHT)) {

		value = false;
	}
	return value;
}