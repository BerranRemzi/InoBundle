#pragma once

#include <stdint.h>

inline bool checkBit(uint8_t _data, uint8_t _pos)
{
    return ((_data >> _pos) & (uint8_t)1);
}

inline void setBit(uint8_t *_data, uint8_t _pos)
{
    *_data |= 1u << _pos;
}

inline void toggleBit(uint8_t *_data, uint8_t _pos)
{
    *_data ^= 1u << _pos;
}

#define LED_ON 0xFF
#define LED_OFF 0x00

#define SCREEN_WIDTH 8
#define SCREEN_HEIGHT 8

#define OUT_OF_SCREEN -1

enum Tick_t
{
    TICK_SLOW = 50,   //slow 		50 x 10 = 500ms
    TICK_MEDIUM = 25, //medium	25 x 10 = 250ms
    TICK_FAST = 5,    //fast		15 x 10 = 150ms
    TICK_2 = 2,       //fast		15 x 10 = 150ms
    TICK_ANIM = 2
};

enum class Sound_t
{
    SNAKE_SILENCE,
    SNAKE_SIZE_UP,
    SNAKE_DEAD,
    SNAKE_MOVE
};

enum class Direction_t
{
    STOPPED,
    UP,
    DOWN,
    RIGHT,
    LEFT
};

enum class Axis_t
{
    AXIS_X,
    AXIS_Y
};

struct Position_t
{
    int8_t x;
    int8_t y;
};

enum class GameState
{
    PAUSE,
    GAME_RUN,
    GAME_WAIT,
    ANIM_RUN,
    ANIM_WAIT,
    BLINK,
    RESTART
};

extern Position_t matrix[SCREEN_WIDTH * SCREEN_HEIGHT];
extern uint8_t matrixSize;