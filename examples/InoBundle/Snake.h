#pragma once

#include "Utility.h"
#include "Arduino.h"
#include "Keyboard.h"
#include "ArcadeBundle.h"

#define SNAKE_START_SIZE 2

class Snake {
public:
	Position_t tail;
	Position_t currentHead;
	Position_t previousHead;
	Position_t food;

	GameState state = GameState::GAME_RUN;
	int8_t size{ 0 };
	Direction_t direction;
	Sound_t sound;
	uint8_t speed{ 0 };
	uint8_t totalTicks;
	bool isFoodGenerated = false;
	Direction_t lastDirection = Direction_t::STOPPED;
public:
	Snake();
	void setup();
	void newGame(void);
	void reset();
	bool isReady();

	void render();
	void update();

	void generateFood();
	void putFood();
	void extendSnake();
	void moveSnake();
	void moveBody();
	bool isInScreen(int i);
	Sound_t playSound();
	Direction_t getLastDirection(void);
};
