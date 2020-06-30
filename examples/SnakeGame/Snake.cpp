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
	for (int i{ 0 }; i < 64; ++i) {
		x[i] = -1;
		y[i] = -1;
	}

	// Start from position (0, 3) and direction is "right"
	head[AXIS_X] = 0;
	head[AXIS_Y] = 3;
	direction = DIR_RIGHT;

	totalTicks = levelTicks[1]; // medium level 250ms
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

	delayTime = SLOW_SPEED_DELAY >> speed;

	sound = DEAD;

	size = 0;

	// Clear the whole screen
	for (int x{ 0 }; x < SCREEN_WIDTH; ++x) {
		for (int y{ 0 }; y < SCREEN_HEIGHT; ++y) {
			setLed(x, y, LED_OFF);
		}
	}
}

uint16_t Snake::loopTime() {
	return delayTime;
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
	setLed((int)head[AXIS_X], (int)head[AXIS_Y], LED_ON);

	// Print food
	setLed((int)food[0], (int)food[1], LED_ON);
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

	for (int i = 0; i < size; ++i) {
		// If head is collided with body
		if (head[AXIS_X] == x[i] && head[AXIS_Y] == y[i]) {
			reset();
		}
	}

	// If head is collided with food
	if (food[0] == head[AXIS_X] && food[1] == head[AXIS_Y]) {
		isFoodGenerated = false;
		generateFood();
		extendSnake();
		sound = SIZE_UP;
	}
	else if (snakeMoved) {
		moveBody();
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
			if (food[0] == x[j] && food[1] == y[j]) {
				putFood();
			}
			else if (head[AXIS_X] == x[j] && head[AXIS_Y] == y[j]) {
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
	//if (direction > 0) {
		head[2] = head[AXIS_X];	// Set old head x to current head x
		head[3] = head[AXIS_Y];	// Set old head y to current head y

		snakeMoved = true;

		sound = MOVE;
	//}

	if(direction == DIR_UP){//if (true == ((direction >> 0) & 1U)) {
		--head[AXIS_Y];
		// If snake head is out of the screen, teleport it to mirrored position
		if (head[AXIS_Y] < 0)
			head[AXIS_Y] = SCREEN_HEIGHT - 1;
	}
	else if (direction == DIR_DOWN) {//else if (true == ((direction >> 1) & 1U)) {
		
		++head[AXIS_Y];
		// If snake head is out of the screen, teleport it to mirrored position
		if (head[AXIS_Y] == SCREEN_HEIGHT)
			head[AXIS_Y] = 0;
	}
	else if (direction == DIR_RIGHT) {//else if (true == ((direction >> 2) & 1U)) {
		direction = DIR_RIGHT;
		++head[AXIS_X];
		// If snake head is out of the screen, teleport it to mirrored position
		if (head[AXIS_X] == SCREEN_WIDTH)
			head[AXIS_X] = 0;
	}
	else if (direction == DIR_LEFT){ //else if (true == ((direction >> 3) & 1U)) {
		direction = DIR_LEFT;
		--head[AXIS_X];
		// If snake head is out of the screen, teleport it to mirrored position
		if (head[AXIS_X] < 0)
			head[AXIS_X] = SCREEN_WIDTH - 1;
	}

	return snakeMoved;
}

Direction_t Snake::getLastDirection(void){
	static Direction_t lastDirection = DIR_STOPPED;

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

Sound_t Snake::playSound() {
	if (sound > SILENCE) {
		sound = SILENCE;
	}

	return sound;
}
