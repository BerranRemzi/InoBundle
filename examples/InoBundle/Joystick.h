
#include "Game.h"
#include "Utility.h"
#include "Arduino.h"
#include "Buttons.h"
#include "ArcadeBundle.h"

#include "Keyboard.h"
#include "Mouse.h"

enum MODE
{
    KEYBOARD,
    MOUSE
};

class Joystick : public Game
{
public:
    uint8_t mouseMoveSpeed = 1;
    bool mode = KEYBOARD;

public:
    Joystick();
    void update() override;
};