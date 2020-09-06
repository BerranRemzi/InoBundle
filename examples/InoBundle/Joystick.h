
#include "Game.h"
#include "Utility.h"
#include "Arduino.h"
#include "Keyboard.h"
#include "ArcadeBundle.h"

//#include "Keyboard.h"
#include "Mouse.h"

const int upButton = 2;
const int downButton = 3;
const int leftButton = 4;
const int rightButton = 5;

class Joystick : public Game
{
public:
    uint8_t mouseMoveSpeed = 1;

public:
    Joystick();
    void update() override;
};