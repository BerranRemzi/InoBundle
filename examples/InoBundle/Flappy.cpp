#include "Flappy.h"

Flappy::Flappy() {
	birdTimer = new Timer(15);
	tubeTimer = new Timer(TICK_MEDIUM * 2);

	setup();
}

void Flappy::setup() {
	bird = { 2, 4 };

	jumpFinished = false;
	isDead = false;

	tubeXSpace = 0;
}

void Flappy::reset() {
	setup();
	ClearScreen();
}


void Flappy::render() {



	// Draw flappy dot
	AB_SetLed(bird.x, bird.y, LED_ON);
}

void Flappy::update() {
	birdTimer->tick();
	tubeTimer->tick();
	bool ready = birdTimer->isReady();

	switch (state) {
	case GameState::PAUSE:
		break;
	case GameState::GAME_RUN:
		state = GameState::GAME_WAIT;

		if ((KB_IsKeyToggled(VK_UP) && KB_IsKeyDown(VK_UP))
			|| (KB_IsKeyToggled(VK_A) && KB_IsKeyDown(VK_A))) {
			jumpHeight = bird.y;
			jumpFinished = false;
		}

		if (jumpHeight - bird.y == 2) {
			jumpFinished = true;
			AB_SetLed(bird.x, bird.y, LED_OFF);
		}
		else if ((KB_IsKeyDown(VK_UP) || KB_IsKeyDown(VK_A))
			&& !jumpFinished) {
			bird.y -= 2;
			AB_SetLed(bird.x, bird.y + 2, LED_OFF);
		}
		else {
			AB_SetLed(bird.x, bird.y, LED_OFF);
		}

		++bird.y;
		if (bird.y == SCREEN_HEIGHT || bird.y == -1)
			reset();

		MoveTubes(tubeTimer->isReady());


		if (AB_GetLed(bird.x, bird.y))
			state = GameState::ANIM_RUN;


		render();
		break;
	case GameState::GAME_WAIT:
		if (ready)
			state = GameState::GAME_RUN;
		break;
	case GameState::ANIM_RUN:
		birdTimer->setTick(TICK_ANIM);
		state = GameState::ANIM_WAIT;

		ready = AB_ClearAnimation();
		if (ready) {
			birdTimer->setTick(15);
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

void Flappy::ClearScreen() {
	// Clear the whole matrix
	for (uint8_t x{ 0 }; x < SCREEN_WIDTH; ++x) {
		for (uint8_t y{ 0 }; y < SCREEN_HEIGHT; ++y) {
			AB_SetLed(x, y, LED_OFF);
		}
	}
}

void Flappy::MoveTubes(bool isReady) {
	if (!isReady)
		return;

	++tubeXSpace;

	// Generate tube 
	if (tubeXSpace == TUBE_X_SPACE) {
		tubeXSpace = 0;

		uint8_t startHeight = random(SCREEN_HEIGHT - 2 - TUBE_Y_SPACE);// Maybe delete "-1"
		for (uint8_t i = 0; i <= startHeight; ++i)
			AB_SetLed(SCREEN_WIDTH - 1, i, LED_ON);

		for (uint8_t i = startHeight + TUBE_Y_SPACE + 1; i < SCREEN_HEIGHT; ++i)
			AB_SetLed(SCREEN_WIDTH - 1, i, LED_ON);
	}

	// Move tubes
	for (uint8_t x{ 0 }; x < SCREEN_WIDTH - 1; ++x) {
		for (uint8_t y{ 0 }; y < SCREEN_HEIGHT; ++y) {
			if (bird.x != x || bird.y != y)
				AB_SetLed(x, y, AB_GetLed(x + 1, y));
		}
	}

	for (uint8_t i = 0; i < SCREEN_HEIGHT; ++i)
		AB_SetLed(7, i, LED_OFF);
}
