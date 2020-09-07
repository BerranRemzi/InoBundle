#pragma once

#include "Game.h"
#include "Arduino.h"
//#include "Utility.h"
//#include "Buttons.h"
#include "ArcadeBundle.h"

class Demo : public Game
{
public:
    Position_t *dots = matrix;

public:
    void update();
};