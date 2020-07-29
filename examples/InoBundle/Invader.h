#pragma once

#include "Game.h"
#include "Utility.h"
#include "Arduino.h"
#include "Keyboard.h"
#include "ArcadeBundle.h"

#define INVADER_START_SIZE	2
#define ROCKET_HEIGHT		2
#define ROCHET_BODY_SIZE	4
#define INVADER_MAX_COUNT	(SCREEN_HEIGHT*ROCKET_HEIGHT)

class Invader : public Game {
public:
    Position_t rocket = { SCREEN_WIDTH / 2, SCREEN_HEIGHT - ROCKET_HEIGHT };
    Position_t rocketBody[ROCHET_BODY_SIZE];
    Position_t* invader = matrix;
    Position_t bullet[SCREEN_HEIGHT - ROCKET_HEIGHT];

    GameState state = GameState::GAME_RUN;
    int8_t size{ 0 };
    Direction_t direction;
    Sound_t sound;
    uint8_t speed{ 0 };
    Timer* timer;
    Timer* invaderSpawner;
public:
    Invader();
    void setup();
    void newGame(void);
    void reset();

    void render();
    void update() override;

    void MoveRocket(Direction_t _direction);
    void DrawFire(bool _fire);
    void DrawRocket();
    void DrawInvader(bool _input);
    void ResetInvader();

    void CollisionBulletInvader();

    Sound_t playSound();
};
