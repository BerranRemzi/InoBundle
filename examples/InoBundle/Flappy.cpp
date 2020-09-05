#include "Flappy.h"

Flappy::Flappy()
{
    keyTimer = new Timer(5);
    birdTimer = new Timer(20);
    tubeTimer = new Timer(TICK_MEDIUM * 2);

    setup();
}

void Flappy::setup()
{
    bird = {2, 4};
    jump = false;
}

void Flappy::reset()
{
    setup();
    ClearScreen();
}

void Flappy::render()
{
    ClearScreen();

    DrawObject(tubes, SCREEN_WIDTH * SCREEN_HEIGHT);

    SetLed(bird.x, bird.y, LED_ON);
}

void Flappy::update()
{
    keyTimer->tick();
    birdTimer->tick();
    tubeTimer->tick();
    bool ready = keyTimer->isReady();

    switch (state)
    {
    case GameState::PAUSE:
        break;
    case GameState::GAME_RUN:
        state = GameState::GAME_WAIT;

        if ((KB_IsKeyToggled(VK_UP) && KB_IsKeyDown(VK_UP)) || (KB_IsKeyToggled(VK_A) && KB_IsKeyDown(VK_A)))
        {
            jump = true;
            jumpHeight = 0;
        }

        if (jump && (KB_IsKeyDown(VK_UP) || KB_IsKeyDown(VK_A)) && jumpHeight != 2)
        {
            --bird.y;
            ++jumpHeight;
        }
        else
        {
            jump = false;
        }

        if (birdTimer->isReady() && !jump)
        {
            ++bird.y;
        }

        if (tubeTimer->isReady())
            MoveTubes();

        if (CheckCollision())
            state = GameState::ANIM_RUN;

        render();
        break;
    case GameState::GAME_WAIT:
        if (ready)
            state = GameState::GAME_RUN;
        break;
    case GameState::ANIM_RUN:
        birdTimer->setTick(TICK_ANIM);
        state = GameState::ANIM_WAIT;

        ready = ClearAnimation();
        if (ready)
        {
            birdTimer->setTick(TICK_MEDIUM);
            state = GameState::RESTART;
        }
        break;
    case GameState::ANIM_WAIT:
        if (ready)
            state = GameState::ANIM_RUN;
        break;
    case GameState::BLINK:
        break;
    case GameState::RESTART:
        reset();
        render();
        state = GameState::GAME_RUN;
        break;
    default:
        break;
    }
}

void Flappy::MoveTubes()
{
    static uint8_t counter = 0;
    ++counter;

    for (uint8_t x{0}; x < SCREEN_WIDTH; ++x)
    {
        for (uint8_t y{1}; y < SCREEN_HEIGHT - 1; ++y)
        {
            uint8_t index = x + y * SCREEN_WIDTH;
            tubes[index].x = tubes[index + SCREEN_WIDTH].x;
            tubes[index].y = y;
        }
    }

    if (counter > 3)
    {
        counter = 0;

        uint8_t startHeight = (uint8_t)random(SCREEN_HEIGHT - TUBE_X_SPACE - 1);

        for (uint8_t i = 0; i <= startHeight; ++i)
        {
            uint8_t index = 7 + i * SCREEN_WIDTH;
            tubes[index].x = 7;
            tubes[index].y = i;
        }
    }
}

bool Flappy::CheckCollision()
{
    bool output = false;

    if (bird.y > SCREEN_HEIGHT - 1 || bird.y < 0)
        output = true;

    return output;
}