#pragma once
#include <stdint.h>
#include <stdint.h>
#include "Arduino.h"

#define LED_ON 0xFF
#define LED_OFF 0x00

#define SCREEN_WIDTH 8
#define SCREEN_HEIGHT 8

class Snake {
public:
	int8_t x[64];
	int8_t y[64];
	int8_t head[2];	// 0 - head X, 1 - head Y
	int8_t food[2];	// 0 - food X, 1 - food Y
public:
	Snake();

	void render();
	void update();

	void generateFood();
	void extendSnake();
	void moveSnake(byte _direction);

	bool isInScreen(int i);
};