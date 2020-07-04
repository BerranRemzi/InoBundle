#include "Snake.h"

extern Position_t matrix[SCREEN_WIDTH * SCREEN_HEIGHT];

Snake::Snake() {
	setup();
}

void Snake::setup() {
	for (int i{ 0 }; i < SCREEN_WIDTH * SCREEN_HEIGHT; ++i) {
		matrix[i] = { OUT_OF_SCREEN, OUT_OF_SCREEN };
	}

	newGame();
	totalTicks = TICK_MEDIUM; // medium level 250ms
}
void Snake::newGame(void) {
	// Start from position (0, 3) and direction is "right"
	currentHead.x = 0;
	currentHead.y = 3;

	size = SNAKE_START_SIZE;

	direction = DIR_RIGHT;
	lastDirection = direction;
	food = { OUT_OF_SCREEN, OUT_OF_SCREEN };
	isFoodGenerated = false;
	generateFood();
}

void Snake::reset() {
	setup();

	if (size > 5) {
		speed++;
	}
	else if (speed > 0) {
		speed--;
	}

	sound = SNAKE_DEAD;
	// Clear the whole matrix
	for (int x{ 0 }; x < SCREEN_WIDTH; ++x) {
		for (int y{ 0 }; y < SCREEN_HEIGHT; ++y) {
			AB_SetLed(x, y, LED_OFF);
		}
	}
}

void Snake::render() {
	// Clear old snake head
	AB_SetLed(previousHead.x, previousHead.y, LED_OFF);

	// Clear old last part of the snake matrix if exists
	if (size > 0)
		AB_SetLed(tail.x, tail.y, LED_OFF);

	// Print snake matrix
	for (int i{ 0 }; i < SCREEN_WIDTH * SCREEN_HEIGHT; ++i) {
		if (isInScreen(i)) {
			AB_SetLed(matrix[i].x, matrix[i].y, LED_ON);
		}
	}

	// Print snake head
	AB_SetLed(currentHead.x, currentHead.y, LED_ON);

	// Print food
	AB_SetLed(food.x, food.y, LED_ON);
}

void Snake::update() {
	static uint8_t cycle = totalTicks;
	if (cycle < totalTicks) {
		generateFood();
		cycle++;
		return;
	}
	cycle = 0;

	Direction_t _direction = getLastDirection();

	if (_direction != DIR_STOPPED) {
		// TODO: Don't overwrite direction, if the "_direction" is
		// opposite of the "direction"
		if (DIR_UP != direction && DIR_DOWN == _direction) {
			direction = _direction;
		}
		if (DIR_DOWN != direction && DIR_UP == _direction) {
			direction = _direction;
		}
		if (DIR_RIGHT != direction && DIR_LEFT == _direction) {
			direction = _direction;
		}
		if (DIR_LEFT != direction && DIR_RIGHT == _direction) {
			direction = _direction;
		}
		_direction = direction;
	}


	bool snakeMoved = moveSnake();



	// If head is collided with food
	if (food.x == currentHead.x && food.y == currentHead.y) {
		isFoodGenerated = false;
		generateFood();
		extendSnake();
		sound = SNAKE_SIZE_UP;
	}
	else if (snakeMoved) {
		moveBody();
	}


	for (int i = 0; i < size; ++i) {
		// If head is collided with matrix
		if (currentHead.x == matrix[i].x && currentHead.y == matrix[i].y) {
			reset();
		}
	}
	// Print to matrix
	render();
	//isFoodGenerated = false;
}

void Snake::generateFood() {
	if (isFoodGenerated == true) {
		return;
	}
	// TODO: Fix food appearing inside of matrix or head,
	// if it appears inside the head, size doesn't grow
	putFood();
	for (int i{ 0 }; i < 16; ++i) {
		for (int j{ 0 }; j < SCREEN_WIDTH * SCREEN_HEIGHT; ++j) {
			if (food.x == matrix[j].x && food.y == matrix[j].y) {
				putFood();
			}
			else if (currentHead.x == matrix[j].x && currentHead.y == matrix[j].y) {
				putFood();
			}
			else {
				isFoodGenerated = true;
				break;
			}
		}
	}
}

void Snake::putFood() {
	food.x = random(8);
	food.y = random(8);
}

void Snake::extendSnake() {
	matrix[size].x = previousHead.x;
	matrix[size].y = previousHead.y;

	++size;
}

bool Snake::moveSnake() {
	bool snakeMoved = false;

	// If snake is going to move
	//if (direction > 0) {
	previousHead.x = currentHead.x;	// Set old head x to current head x
	previousHead.y = currentHead.y;	// Set old head y to current head y

	snakeMoved = true;

	sound = SNAKE_MOVE;
	//}

	if (direction == DIR_UP) {//if (true == ((direction >> 0) & 1U)) {
		--currentHead.y;
		// If snake head is out of the matrix, teleport it to mirrored position
		if (currentHead.y < 0)
			currentHead.y = SCREEN_HEIGHT - 1;
	}
	else if (direction == DIR_DOWN) {//else if (true == ((direction >> 1) & 1U)) {

		++currentHead.y;
		// If snake head is out of the matrix, teleport it to mirrored position
		if (currentHead.y == SCREEN_HEIGHT)
			currentHead.y = 0;
	}
	else if (direction == DIR_RIGHT) {//else if (true == ((direction >> 2) & 1U)) {
		direction = DIR_RIGHT;
		++currentHead.x;
		// If snake head is out of the matrix, teleport it to mirrored position
		if (currentHead.x == SCREEN_WIDTH)
			currentHead.x = 0;
	}
	else if (direction == DIR_LEFT) { //else if (true == ((direction >> 3) & 1U)) {
		direction = DIR_LEFT;
		--currentHead.x;
		// If snake head is out of the matrix, teleport it to mirrored position
		if (currentHead.x < 0)
			currentHead.x = SCREEN_WIDTH - 1;
	}

	return snakeMoved;
}

Direction_t Snake::getLastDirection(void) {


	if (KB_IsPressed(VK_UP) && DIR_UP != lastDirection) {
		lastDirection = DIR_UP;
	}

	if (KB_IsPressed(VK_DOWN) && DIR_DOWN != lastDirection) {
		lastDirection = DIR_DOWN;
	}

	if (KB_IsPressed(VK_RIGHT) && DIR_RIGHT != lastDirection) {
		lastDirection = DIR_RIGHT;
	}

	if (KB_IsPressed(VK_LEFT) && DIR_LEFT != lastDirection) {
		lastDirection = DIR_LEFT;
	}
	KB_Reset();
	return lastDirection;
}

void Snake::moveBody() {
	if (size > 0) {
		tail.x = matrix[0].x;
		tail.y = matrix[0].y;

		for (int i{ 0 }; i < size - 1; ++i) {
			matrix[i].x = matrix[i + 1].x;
			matrix[i].y = matrix[i + 1].y;
		}

		matrix[size - 1].x = previousHead.x;
		matrix[size - 1].y = previousHead.y;
	}
}

bool Snake::isInScreen(int i) {
	bool value{ true };

	/*
	if ((x[i] < 0 || x[i] >= matrix_WIDTH) ||
		(y[i] < 0 || y[i] >= matrix_HEIGHT)) {
		value = false;
	}
	*/

	if (matrix[i].x == OUT_OF_SCREEN || matrix[i].y == OUT_OF_SCREEN)
		value = false;

	return value;
}

Sound_t Snake::playSound() {
	if (sound > SNAKE_SILENCE) {
		sound = SNAKE_SILENCE;
	}

	return sound;
}
