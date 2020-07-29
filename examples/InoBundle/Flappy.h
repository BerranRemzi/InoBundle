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
    uint8_t jumpHeight;
    bool jump;

    Position_t* tubes = matrix;

    GameState state = GameState::GAME_RUN;
    Sound_t sound;

    Timer* keyTimer;
    Timer* birdTimer;
    Timer* tubeTimer;
public:
    Flappy();
    void setup();
    void reset();

    void render();
    void update() override;

    void MoveTubes();
    bool CheckCollision();
};