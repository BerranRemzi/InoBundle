#include "Snake.h"

Snake::Snake() {
	setup();
}

void Snake::setup() {
	for (int i{ 0 }; i < 64; ++i) {
		x[i] = -1;
		y[i] = -1;
	}

	// Start from position (0, 3) and direction is "right"
	head[0] = 0;
	head[1] = 3;
	direction = 0b0100;

	generateFood();
}

void Snake::reset() {
	setup();

	size = 0;

	// Clear the whole screen
	for (int x{ 0 }; x < SCREEN_WIDTH; ++x) {
		for (int y{ 0 }; y < SCREEN_HEIGHT; ++y) {
			setLed(x, y, LED_OFF);
		}
	}
}

void Snake::render() {
	// Clear old snake head
	setLed((int)head[2], (int)head[3], LED_OFF);

	// Clear old last part of the snake body if exists
	if (size > 0)
		setLed(bodyLast[0], bodyLast[1], LED_OFF);

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
	if (_direction != false) {
		// TODO: Don't overwrite direction, if the "_direction" is
		// opposite of the "direction"
		direction = _direction;
	}

	bool snakeMoved = moveSnake();

	for (int i = 0; i < size; ++i) {
		// If head is collided with body
		if (head[0] == x[i] && head[1] == y[i]) {
			reset();
		}
	}

	// If head is collided with food
	if (food[0] == head[0] && food[1] == head[1]) {
		generateFood();
		extendSnake();
	}
	else if (snakeMoved) {
		moveBody();
	}
}

void Snake::generateFood() {
	// TODO: Fix food appearing inside of body or head,
	// if it appears inside the head, size doesn't grow
	food[0] = random(8);
	food[1] = random(8);
}

void Snake::extendSnake() {
	x[size] = head[2];
	y[size] = head[3];

	++size;
}

bool Snake::moveSnake() {
	bool snakeMoved = false;

	// If snake is going to move
	if (direction > 0) {
		head[2] = head[0];	// Set old head x to current head x
		head[3] = head[1];	// Set old head y to current head y

		snakeMoved = true;
	}

	if (true == ((direction >> 0) & 1U)) {
		--head[1];
		// If snake head is out of the screen, teleport it to mirrored position
		if (head[1] < 0)
			head[1] = SCREEN_HEIGHT - 1;
	}
	else if (true == ((direction >> 1) & 1U)) {
		++head[1];
		// If snake head is out of the screen, teleport it to mirrored position
		if (head[1] == SCREEN_HEIGHT)
			head[1] = 0;
	}
	else if (true == ((direction >> 2) & 1U)) {
		++head[0];
		// If snake head is out of the screen, teleport it to mirrored position
		if (head[0] == SCREEN_WIDTH)
			head[0] = 0;
	}
	else if (true == ((direction >> 3) & 1U)) {
		--head[0];
		// If snake head is out of the screen, teleport it to mirrored position
		if (head[0] < 0)
			head[0] = SCREEN_WIDTH - 1;
	}

	return snakeMoved;
}

void Snake::moveBody() {
	if (size > 0) {
		bodyLast[0] = x[0];
		bodyLast[1] = y[0];

		for (int i{ 0 }; i < size - 1; ++i) {
			x[i] = x[i + 1];
			y[i] = y[i + 1];
		}

		x[size - 1] = head[2];
		y[size - 1] = head[3];
	}
}

bool Snake::isInScreen(int i) {
	bool value{ true };

	/*
	if ((x[i] < 0 || x[i] >= SCREEN_WIDTH) ||
		(y[i] < 0 || y[i] >= SCREEN_HEIGHT)) {
		value = false;
	}
	*/

	if (x[i] == -1 || y[i] == -1)
		value = false;

	return value;
}