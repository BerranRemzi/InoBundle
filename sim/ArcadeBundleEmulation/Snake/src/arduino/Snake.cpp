#include "Snake.h"

Snake::Snake() {
	for (int i{ 0 }; i < 64; ++i) {
		x[i] = -1;
		y[i] = -1;
	}

	// 3 4 right
	head[0] = random(8);
	head[1] = random(8);

	generateFood();
}

void Snake::render() {
	// Clear old snake head
	setLed((int)head[2], (int)head[3], LED_OFF);

	// Clear old last snake body if exists
	if (size > 0)
		setLed(x[size - 1] - (head[0] - head[2]), y[size - 1] - (head[1] - head[3]), LED_OFF);

	// Print snake body
	for (int i{ 0 }; i < SCREEN_WIDTH * SCREEN_HEIGHT; ++i) {
		if (isInScreen(i)) {
			setLed(x[i], y[i], LED_ON);
		}
	}

	// Print snake head
	setLed((int)head[0], (int)head[1], LED_ON);

	// Print food
	setLed((int)food[0], (int)food[1], LED_ON);
}

void Snake::update(byte _direction) {
	moveSnake(_direction);

	if (food[0] == head[0] && food[1] == head[1]) {
		generateFood();
		extendSnake();
		moveBody();
	}
}

void Snake::generateFood() {
	food[0] = random(8);
	food[1] = random(8);
}

void Snake::extendSnake() {
	x[size] = head[2];
	y[size] = head[3];

	++size;
}

void Snake::moveSnake(byte _direction) {
	// If snake is going to move
	if (_direction > 0) {
		head[2] = head[0];	// Set old head x to current head x
		head[3] = head[1];	// Set old head y to current head y
	}

	if (true == ((_direction >> 0) & 1U)) {
		--head[1];
	}
	else if (true == ((_direction >> 1) & 1U)) {
		++head[1];
	}
	else if (true == ((_direction >> 2) & 1U)) {
		++head[0];
	}
	else if (true == ((_direction >> 3) & 1U)) {
		--head[0];
	}

	// If snake has moved
	if (_direction > 0) {
		moveBody();
	}
}

void Snake::moveBody() {
	if (size > 0) {
		for (int i{ size - 1 }; i > 0; --i) {
			x[size] = x[size - 1];
			y[size] = y[size - 1];
		}
		x[0] = head[2];
		y[0] = head[3];
	}
}

bool Snake::isInScreen(int i) {
	bool value{ true };
	if ((x[i] < 0 || x[i] >= SCREEN_WIDTH) ||
		(y[i] < 0 || y[i] >= SCREEN_HEIGHT)) {
		value = false;
	}
	return value;
}