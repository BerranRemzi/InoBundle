#pragma once
#include <stdint.h>
#include <stdint.h>
#include "Arduino.h"
#include <iostream>

#define LED_ON 0xFF
#define LED_OFF 0x00

#define SCREEN_WIDTH 8
#define SCREEN_HEIGHT 8

#define SLOW_SPEED_DELAY 0x1FF /* 511 */

enum Sound_t {
	SILENCE,
	SIZE_UP,
	DEAD,
	MOVE
};

class Snake {
public:
	int8_t x[64];		// Snake body X
	int8_t y[64];		// Snake body Y
	int8_t head[4];		// 0 - head X, 1 - head Y, 2 - old head X, 3 - old head Y
	int8_t bodyLast[2];	// 0 - last body X, 1 - last body Y 
	int8_t food[2];		// 0 - food X, 1 - food Y
	int8_t size{ 0 };
	byte direction;
	Sound_t sound;
	uint16_t delayTime{ SLOW_SPEED_DELAY };
	uint8_t speed{ 0 };
public:
	Snake();

	void setup();
	void reset();

	void render();
	void update(byte _direction);

	void generateFood();
	void putFood();
	void extendSnake();
	bool moveSnake();
	void moveBody();
	uint16_t loopTime();
	bool isInScreen(int i);
	Sound_t playSound();
};