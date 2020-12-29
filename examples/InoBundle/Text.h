#pragma once

#include "Game.h"
#include "Arduino.h"
//#include "Utility.h"
//#include "Buttons.h"
#include "ArcadeBundle.h"

class Text : public Game
{
private:
    Position_t *dots = matrix;
    GameState state = GameState::GAME_RUN;
    Timer* frame;
    void scrollText(const char* p);
public:
    Text();
    void update();
};
