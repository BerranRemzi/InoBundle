#pragma once

#include "ArcadeBundle_Types.h"
#include "Keyboard.h"
#include "Debug.h"

#ifdef ARDUINO

#include <avr/pgmspace.h>
#include "ArcadeBundle_HAL.h"

#else

#define AB_HAL_Init()

extern void AB_HAL_UpdateScreen(void);

#endif

class Timer
{
private:
    bool returnValue = false;
    uint8_t totalTicks;
    uint8_t cycle;

public:
    explicit Timer(uint8_t _totalTicks);
    void tick(void);
    void setTick(uint8_t _tick);
    uint8_t getTick();
    bool isReady(void);
};

class Game
{
public:
    virtual void update() = 0;

    static void InitScreen(void);
    static void Setup(void);

    static void SetLed(uint8_t _x, uint8_t _y, uint8_t _brightness);
    static uint8_t GetLed(uint8_t _x, uint8_t _y);
    static void SetRow(uint8_t _y, uint8_t _value);
    static void ClearDisplay(void);
    static void ClearScreen(void);

    static void UpdateScreen(void);

    static bool ClearAnimation(void);
    static bool IsInScreen(Position_t *_object, uint8_t _pos);

    static void DrawObject(Position_t *_object, uint8_t _size);
    static void FillObject(Position_t *_object, uint8_t _size, uint8_t _data);
    static bool CollisionDetect(Position_t *_objectA, uint8_t _sizeA, Position_t *_objectB, uint8_t _sizeB);
    static bool CopyObject(Position_t *_objectA, uint8_t _sizeA, Position_t *_objectB, uint8_t _sizeB);
    static Direction_t ReadDirection(void);

    static void DeleteRow(uint8_t _row);
};
