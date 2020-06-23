#pragma once

#include <stdlib.h>
#include <ctime>

class Snake {
public:
	unsigned char body[64 * 2];
	unsigned char head[2];	// 0 - head X, 1 - head Y
	unsigned char food[2];	// 0 - food X, 1 - food Y
public:
	Snake();

	void generateFood();
	void extendSnake();
	void moveSnake(int direction);
};