#include "Snake.h"

Snake::Snake() {
	timer = new Timer(TICK_MEDIUM);
	setup();
}

void Snake::setup() {
	for (int i{ 0 }; i < SCREEN_WIDTH * SCREEN_HEIGHT; ++i) {
		matrix[i] = { OUT_OF_SCREEN, OUT_OF_SCREEN };
	}

	newGame();
}
void Snake::newGame(void) {
	// Start from position (0, 3) and direction is "right"
	currentHead.x = 0;
	currentHead.y = 3;

	size = SNAKE_START_SIZE;

	direction = Direction_t::RIGHT;
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

	sound = Sound_t::SNAKE_DEAD;
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
	timer->tick();
	bool ready = timer->isReady();

	switch (state) {
	case GameState::PAUSE:
		break;
	case GameState::GAME_RUN:
	{
		timer->setTick(TICK_MEDIUM);
		state = GameState::GAME_WAIT;

		Direction_t _direction = getLastDirection();

		if (_direction != Direction_t::STOPPED) {
			// TODO: Don't overwrite direction, if the "_direction" is
			// opposite of the "direction"
			if (Direction_t::UP != direction && Direction_t::DOWN == _direction) {
				direction = _direction;
			}
			if (Direction_t::DOWN != direction && Direction_t::UP == _direction) {
				direction = _direction;
			}
			if (Direction_t::RIGHT != direction && Direction_t::LEFT == _direction) {
				direction = _direction;
			}
			if (Direction_t::LEFT != direction && Direction_t::RIGHT == _direction) {
				direction = _direction;
			}
		}

		moveSnake();

		// If head is collided with food
		if (food.x == currentHead.x && food.y == currentHead.y) {
			isFoodGenerated = false;
			generateFood();
			extendSnake();
			sound = Sound_t::SNAKE_SIZE_UP;
		}
		else {
			moveBody();
		}


		for (int i = 0; i < size; ++i) {
			// If head is collided with matrix
			if (currentHead.x == matrix[i].x && currentHead.y == matrix[i].y) {
				state = GameState::ANIM_RUN;
			}
		}
		// Print to matrix
		render();
	}
	break;
	case GameState::GAME_WAIT:
		if (ready)
			state = GameState::GAME_RUN;
		break;
	case GameState::ANIM_RUN:
		timer->setTick(TICK_ANIM);
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
	default:
		break;
	}


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

void Snake::moveSnake() {

	// If snake is going to move
	previousHead.x = currentHead.x;	// Set old head x to current head x
	previousHead.y = currentHead.y;	// Set old head y to current head y

	sound = Sound_t::SNAKE_MOVE;

	if (direction == Direction_t::UP) {
		--currentHead.y;
		// If snake head is out of the matrix, teleport it to mirrored position
		if (currentHead.y < 0)
			currentHead.y = SCREEN_HEIGHT - 1;
	}
	else if (direction == Direction_t::DOWN) {

		++currentHead.y;
		// If snake head is out of the matrix, teleport it to mirrored position
		if (currentHead.y == SCREEN_HEIGHT)
			currentHead.y = 0;
	}
	else if (direction == Direction_t::RIGHT) {
		direction = Direction_t::RIGHT;
		++currentHead.x;
		// If snake head is out of the matrix, teleport it to mirrored position
		if (currentHead.x == SCREEN_WIDTH)
			currentHead.x = 0;
	}
	else if (direction == Direction_t::LEFT) {
		direction = Direction_t::LEFT;
		--currentHead.x;
		// If snake head is out of the matrix, teleport it to mirrored position
		if (currentHead.x < 0)
			currentHead.x = SCREEN_WIDTH - 1;
	}
}

Direction_t Snake::getLastDirection(void) {


	if (KB_IsKeyPressed(VK_UP) && Direction_t::UP != lastDirection) {
		lastDirection = Direction_t::UP;
	}

	if (KB_IsKeyPressed(VK_DOWN) && Direction_t::DOWN != lastDirection) {
		lastDirection = Direction_t::DOWN;
	}

	if (KB_IsKeyPressed(VK_RIGHT) && Direction_t::RIGHT != lastDirection) {
		lastDirection = Direction_t::RIGHT;
	}

	if (KB_IsKeyPressed(VK_LEFT) && Direction_t::LEFT != lastDirection) {
		lastDirection = Direction_t::LEFT;
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

	if (matrix[i].x == OUT_OF_SCREEN || matrix[i].y == OUT_OF_SCREEN)
		value = false;

	return value;
}

Sound_t Snake::playSound() {
	if (sound > Sound_t::SNAKE_SILENCE) {
		sound = Sound_t::SNAKE_SILENCE;
	}

	return sound;
}
