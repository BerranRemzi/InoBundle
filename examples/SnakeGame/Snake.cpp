#include "Snake.h"
#include "Keyboard.h"

uint8_t levelTicks[] = {
	50, //slow 		50 x 10 = 500ms
	25, //medium	25 x 10 = 250ms
	15  //fast		15 x 10 = 150ms
};

Snake::Snake() {
	setup();
}

void Snake::setup() {
	for (int i{ 0 }; i < SCREEN_WIDTH * SCREEN_HEIGHT; ++i) {
		body[i] = {OUT_OF_SCREEN, OUT_OF_SCREEN};
	}

	newGame();
	totalTicks = levelTicks[1]; // medium level 250ms
}
void Snake::newGame(void){
	// Start from position (0, 3) and direction is "right"
	currentHead.x = 0;
	currentHead.y = 3;

	size = SNAKE_START_SIZE;

	direction = DIR_RIGHT;
	lastDirection = direction;
	food = {OUT_OF_SCREEN, OUT_OF_SCREEN};
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

	sound = DEAD;
	// Clear the whole screen
	for (int x{ 0 }; x < SCREEN_WIDTH; ++x) {
		for (int y{ 0 }; y < SCREEN_HEIGHT; ++y) {
			setLed(x, y, LED_OFF);
		}
	}
}

void Snake::render() {
	// Clear old snake head
	setLed(oldHead.x, oldHead.y, LED_OFF);

	// Clear old last part of the snake body if exists
	if (size > 0)
		setLed(bodyLast.x, bodyLast.y, LED_OFF);

	// Print snake body
	for (int i{ 0 }; i < SCREEN_WIDTH * SCREEN_HEIGHT; ++i) {
		if (isInScreen(i)) {
			setLed(body[i].x, body[i].y, LED_ON);
		}
	}

	// Print snake head
	setLed(currentHead.x, currentHead.y, LED_ON);

	// Print food
	setLed(food.x, food.y, LED_ON);
}

void Snake::update() {
	static uint8_t cycle = totalTicks;
	if(cycle < totalTicks){
		generateFood();
		cycle++;
		return;
	}
	cycle = 0;

	Direction_t _direction = getLastDirection();
	
	if (_direction != DIR_STOPPED) {
		// TODO: Don't overwrite direction, if the "_direction" is
		// opposite of the "direction"
		if(DIR_UP != direction && DIR_DOWN == _direction){
			direction = _direction;
		}
		if(DIR_DOWN != direction && DIR_UP == _direction){
			direction = _direction;
		}
		if(DIR_RIGHT != direction && DIR_LEFT == _direction){
			direction = _direction;
		}
		if(DIR_LEFT != direction && DIR_RIGHT == _direction){
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
		sound = SIZE_UP;
	}
	else if (snakeMoved) {
		moveBody();
	}


	for (int i = 0; i < size; ++i) {
		// If head is collided with body
		if (currentHead.x == body[i].x && currentHead.y == body[i].y) {
			reset();
		}
	}
	// Print to screen
	render();
	//isFoodGenerated = false;
}

void Snake::generateFood() {
	if(isFoodGenerated == true){
		return;
	}
	// TODO: Fix food appearing inside of body or head,
	// if it appears inside the head, size doesn't grow
	putFood();
	for (int i{ 0 }; i < 16; ++i) {
		for (int j{ 0 }; j < SCREEN_WIDTH * SCREEN_HEIGHT; ++j) {
			if (food.x == body[j].x && food.y == body[j].y) {
				putFood();
			}
			else if (currentHead.x == body[j].x && currentHead.y == body[j].y) {
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
	body[size].x = oldHead.x;
	body[size].y = oldHead.y;

	++size;
}

bool Snake::moveSnake() {
	bool snakeMoved = false;

	// If snake is going to move
	//if (direction > 0) {
		oldHead.x = currentHead.x;	// Set old head x to current head x
		oldHead.y = currentHead.y;	// Set old head y to current head y

		snakeMoved = true;

		sound = MOVE;
	//}

	if(direction == DIR_UP){//if (true == ((direction >> 0) & 1U)) {
		--currentHead.y;
		// If snake head is out of the screen, teleport it to mirrored position
		if (currentHead.y < 0)
			currentHead.y = SCREEN_HEIGHT - 1;
	}
	else if (direction == DIR_DOWN) {//else if (true == ((direction >> 1) & 1U)) {
		
		++currentHead.y;
		// If snake head is out of the screen, teleport it to mirrored position
		if (currentHead.y == SCREEN_HEIGHT)
			currentHead.y = 0;
	}
	else if (direction == DIR_RIGHT) {//else if (true == ((direction >> 2) & 1U)) {
		direction = DIR_RIGHT;
		++currentHead.x;
		// If snake head is out of the screen, teleport it to mirrored position
		if (currentHead.x == SCREEN_WIDTH)
			currentHead.x = 0;
	}
	else if (direction == DIR_LEFT){ //else if (true == ((direction >> 3) & 1U)) {
		direction = DIR_LEFT;
		--currentHead.x;
		// If snake head is out of the screen, teleport it to mirrored position
		if (currentHead.x < 0)
			currentHead.x = SCREEN_WIDTH - 1;
	}

	return snakeMoved;
}

Direction_t Snake::getLastDirection(void){
	

	if(KB_IsPressed(VK_UP) && DIR_UP != lastDirection){
		lastDirection = DIR_UP;
	}

	if(KB_IsPressed(VK_DOWN) && DIR_DOWN != lastDirection){
		lastDirection = DIR_DOWN;
	}

	if(KB_IsPressed(VK_RIGHT) && DIR_RIGHT != lastDirection){
		lastDirection = DIR_RIGHT;
	}

	if(KB_IsPressed(VK_LEFT) && DIR_LEFT != lastDirection){
		lastDirection = DIR_LEFT;
	}
	KB_Reset();
	return lastDirection;
}

void Snake::moveBody() {
	if (size > 0) {
		bodyLast.x = body[0].x;
		bodyLast.y = body[0].y;

		for (int i{ 0 }; i < size - 1; ++i) {
			body[i].x = body[i + 1].x;
			body[i].y = body[i + 1].y;
		}

		body[size - 1].x = oldHead.x;
		body[size - 1].y = oldHead.y;
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

	if (body[i].x == OUT_OF_SCREEN || body[i].x == OUT_OF_SCREEN)
		value = false;

	return value;
}

Sound_t Snake::playSound() {
	if (sound > SILENCE) {
		sound = SILENCE;
	}

	return sound;
}
