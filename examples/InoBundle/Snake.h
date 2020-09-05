#pragma once

#include "Game.h"
#include "Utility.h"
#include "Arduino.h"
#include "Keyboard.h"
#include "ArcadeBundle.h"

#define SNAKE_START_SIZE        2u
#define TOTAL_FOOD_TRY_COUNT    32u

class Snake : public Game {
public:
    Position_t tail;
    Position_t currentHead;
    Position_t previousHead;
    Position_t food;

    GameState state = GameState::GAME_RUN;
    int8_t size{ 0 };

    Sound_t sound;
    uint8_t speed{ 0 };
    bool isFoodGenerated = false;
    Direction_t lastDirection = Direction_t::STOPPED;
    Timer* timer;
public:
    Snake();
    void setup();
    void newGame(void);
    void reset(void);

    const void render(void);
    void update() override;

    void generateFood(void);
    void putFood(void);
    void extendSnake(void);
    void MoveSnake(Direction_t _newDirection);
    void moveBody(void);

    Sound_t playSound(void);
    Direction_t getLastDirection(void);
    const Direction_t GetValidDirection(Direction_t _currentDirection);
};
