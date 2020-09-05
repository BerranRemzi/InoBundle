#pragma once

#include "Game.h"
#include "Arduino.h"
#include "Utility.h"
#include "Keyboard.h"
#include "ArcadeBundle.h"

#define LED_PERIOD 10
#define LED_TIME 8
#define COLUMN_COUNT 3
#define POINT_COUNT (COLUMN_COUNT * SCREEN_WIDTH)

enum
{
    NUM_1,
    OPERAND,
    NUM_2,
    RESULT
};

class Calculator : public Game
{
public:
    GameState state = GameState::GAME_RUN;
    uint8_t screen[POINT_COUNT] = {};
    uint8_t buffer[4]{};

    Position_t cursor;
    Timer *timer;
    Timer *blinkTimer;
    Timer *heldTimer;
    uint8_t blinkCounter;

public:
    Calculator();
    void reset();

    void update() override;
    void render();

    void calculate();
};