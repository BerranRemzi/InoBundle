#include "Calculator.h"

Calculator::Calculator()
{
    timer = new Timer(TICK_FAST);
    blinkTimer = new Timer(LED_PERIOD);
    reset();
}

void Calculator::reset()
{
    cursor = {SCREEN_WIDTH - 1, 0};

    blinkCounter = 0;
}

void Calculator::update()
{
    timer->tick();
    blinkTimer->tick();
    bool ready = timer->isReady();

    switch (state)
    {
    case GameState::PAUSE:
        break;
    case GameState::GAME_RUN:
        state = GameState::GAME_WAIT;

        if (KB_IsSinglePressed(VK_UP))
        {
            cursor.y--;
            if (cursor.y < 0)
            {
                cursor.y = 2;
            }
        }
        else if (KB_IsSinglePressed(VK_DOWN))
        {
            cursor.y++;
            if (cursor.y > 2)
            {
                cursor.y = 0;
            }
        }
        else if (KB_IsSinglePressed(VK_LEFT))
        {
            cursor.x--;
            if (cursor.x < 0)
            {
                cursor.x = SCREEN_WIDTH - 1;
            }
        }
        else if (KB_IsSinglePressed(VK_RIGHT))
        {
            cursor.x++;
            if (cursor.x > SCREEN_WIDTH - 1)
            {
                cursor.x = 0;
            }
        }

        if (KB_IsSinglePressed(VK_X))
        {
            toggleBit(&buffer[cursor.y], SCREEN_WIDTH - 1 - cursor.x);
        }
        if (KB_IsSinglePressed(VK_Y))
        {
            buffer[cursor.y]++;
        }
        if (KB_IsSinglePressed(VK_A))
        {
            buffer[cursor.y]--;
        }
        if (KB_IsSinglePressed(VK_B))
        {
            calculate();
            buffer[NUM_1] = buffer[RESULT];
            buffer[OPERAND] = 0;
            buffer[NUM_2] = 0;
            buffer[RESULT] = 0;
        }

        calculate();
        render();
        break;
    case GameState::GAME_WAIT:
        if (ready)
            state = GameState::GAME_RUN;
        break;
    case GameState::ANIM_RUN:
        timer->setTick(TICK_ANIM);
        state = GameState::ANIM_WAIT;

        ready = ClearAnimation();
        if (ready)
        {
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
    }
}

void Calculator::render()
{
#if 0
    for (uint8_t y = 0; y < COLUMN_COUNT; ++y)
    {
        for (uint8_t x = 0; x < SCREEN_WIDTH; ++x)
        {
            if (cursor.x != x || cursor.y != y)
                SetLed(x, y, screen[x + y * SCREEN_WIDTH]);
            else if (cursor.x == x && cursor.y == y)
            {
            }
            else if (blinkCounter > 2 && screen[cursor.x + cursor.y * SCREEN_WIDTH] == LED_ON)
                SetLed(cursor.x, cursor.y, LED_OFF);
            else
                SetLed(cursor.x, cursor.y, LED_ON);
        }
    }
#endif
    for (uint8_t x = 0; x < SCREEN_WIDTH; ++x)
    {
        uint8_t _x = SCREEN_WIDTH - x - 1;
        for (uint8_t y = 0; y < sizeof(buffer); ++y)
        {
            SetLed(_x, y, checkBit(buffer[y], x) == true ? LED_ON : LED_OFF);
        }
    }
    if (blinkCounter > LED_TIME)
    {
        if (GetLed(cursor.x, cursor.y) == LED_OFF)
        {
            SetLed(cursor.x, cursor.y, LED_ON);
        }
        else
        {
            SetLed(cursor.x, cursor.y, LED_OFF);
        }
    }
    blinkCounter++;
    if (blinkCounter > LED_PERIOD)
        blinkCounter = 0;

    //if (blinkTimer->isReady())
    //{
    //    blinkTimer->setTick(100 - blinkTimer->getTick());
    //}
}

void Calculator::calculate()
{
    switch (buffer[OPERAND])
    {
    case 1:
        buffer[RESULT] = buffer[NUM_1] + buffer[NUM_2];
        break;
    case 2:
        buffer[RESULT] = buffer[NUM_1] - buffer[NUM_2];
        break;
    case 4:
        buffer[RESULT] = buffer[NUM_1] * buffer[NUM_2];
        break;
    case 8:
        if (buffer[NUM_2] > 0)
            buffer[RESULT] = buffer[NUM_1] / buffer[NUM_2];
        else
            buffer[RESULT] = 0xFF;
        break;
    case 16:
        buffer[RESULT] = buffer[NUM_1] & buffer[NUM_2];
        break;
    case 32:
        buffer[RESULT] = buffer[NUM_1] | buffer[NUM_2];
        break;
    case 64:
        buffer[RESULT] = buffer[NUM_1] ^ buffer[NUM_2];
        break;
    case 128:
        buffer[RESULT] = buffer[NUM_1] % buffer[NUM_2];
        break;
    default:
        break;
    }
}
