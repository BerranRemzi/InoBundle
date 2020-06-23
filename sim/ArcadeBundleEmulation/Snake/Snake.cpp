#include "Snake.h"

Snake::Snake() {
	srand(time(nullptr));

	for (int i{ 0 }; i < 64; ++i) {
		x[i] = -1;
		y[i] = -1;
	}

	head[0] = rand() % 8;
	head[1] = rand() % 8;

	generateFood();
}

void Snake::generateFood() {
	food[0] = rand() % 8;
	food[1] = rand() % 8;
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