#pragma once

#include "Game.h"
#include "Utility.h"
#include "Arduino.h"
#include "Keyboard.h"
#include "ArcadeBundle.h"

#define TUBE_X_SPACE 3
#define TUBE_Y_SPACE 3

class Flappy : public Game {
public:
	Position_t bird;
	bool isDead;

	bool jumpFinished;
	bool jumped;
	uint8_t jumpHeight;

	uint8_t tubeXSpace;

	GameState state = GameState::GAME_RUN;
	Sound_t sound;

	Timer* birdTimer;
	Timer* tubeTimer;
public:
	Flappy();
	void setup();
	void reset();

	void render();
	void update() override;

	void ClearScreen();

	void MoveTubes(bool isReady);
};