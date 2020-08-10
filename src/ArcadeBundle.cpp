#include "ArcadeBundle.h"

uint8_t AB_screen[8][8];
Position_t matrix[SCREEN_WIDTH * SCREEN_HEIGHT];
uint8_t matrixSize = SCREEN_WIDTH * SCREEN_HEIGHT;

Timer::Timer(uint8_t _totalTicks) {
    totalTicks = _totalTicks;
    cycle = 0;
}

void Timer::tick(void) {
    if (cycle < totalTicks) {
        cycle++;
    }
    else {
        cycle = 0;
        returnValue = true;
    }
}

bool Timer::isReady(void) {
    if (true == returnValue) {
        returnValue = false;
        return true;
    }

    return false;
}

void Timer::setTick(uint8_t _tick) {
    totalTicks = _tick;
}

void Game::InitScreen(void)
{
    AB_HAL_Init();
}

void Game::Setup(void)
{
    randomSeed(analogRead(7));
    KB_Setup();
    InitScreen();
}

void Game::SetLed(uint8_t _x, uint8_t _y, uint8_t _brightness)
{
    if (_y < SCREEN_HEIGHT && _x < SCREEN_WIDTH) {
        AB_screen[_y][_x] = _brightness;
    }
}

uint8_t Game::GetLed(uint8_t _x, uint8_t _y)
{
    uint8_t output;

    if (_y < SCREEN_HEIGHT && _x < SCREEN_WIDTH) {
        output = AB_screen[_y][_x];
    }
    else {
        output = LED_OFF;
    }

    return output;
}

void Game::SetRow(uint8_t _y, uint8_t _value)
{
    for (int x = 0; x < 8; x++) {
        if ((_value >> x) & 1U) {
            AB_screen[_y][x] = LED_ON;
        }
        else {
            AB_screen[_y][x] = LED_OFF;
        }
    }
}

bool Game::ClearAnimation(void)
{
    for (int8_t x = SCREEN_WIDTH - 1; x >= 0; x--) {
        for (int8_t y = 0; y < SCREEN_HEIGHT; y++) {
            if ((AB_screen[x][y] == LED_ON) && (x < (SCREEN_WIDTH - 1))) {
                AB_screen[x + 1][y] = AB_screen[x][y];
            }

            if (AB_screen[x][y] == LED_ON) {
                AB_screen[x][y] = LED_OFF;
                /* return false if some of leds was ON */
                return false;
            }
        }
    }
    /* return true if all leds are OFF*/
    return true;
}

void Game::ClearDisplay(void)
{
    for (uint8_t y = 0; y < 8; y++) {
        for (uint8_t x = 0; x < 8; x++) {
            AB_screen[x][y] = LED_OFF;
        }
    }
}

void Game::ClearScreen(void)
{
    // Clear the whole matrix
    for (uint8_t x = 0; x < SCREEN_WIDTH; ++x) {
        for (uint8_t y = 0; y < SCREEN_HEIGHT; ++y) {
            SetLed(x, y, LED_OFF);
        }
    }
}

void Game::UpdateScreen(void)
{
    AB_HAL_UpdateScreen();
}

bool Game::IsInScreen(Position_t* _object, uint8_t _pos)
{
    bool value = false;

    if (_object[_pos].x >= 0 && _object[_pos].x < SCREEN_WIDTH
        && _object[_pos].y >= 0 && _object[_pos].y < SCREEN_HEIGHT) {
        value = true;
    }
    else {
        _object[_pos] = { OUT_OF_SCREEN, OUT_OF_SCREEN };
    }

    return value;
}

void Game::DrawObject(Position_t* _object, uint8_t _size)
{
    for (uint8_t i = 0; i < _size; ++i) {
        if (IsInScreen(_object, i)) {
            SetLed(_object[i].x, _object[i].y, LED_ON);
        }
    }
}

void Game::FillObject(Position_t* _object, uint8_t _size, uint8_t _data)
{
    for (uint8_t i = 0; i < _size; ++i) {
        if (IsInScreen(_object, i)) {
            SetLed(_object[i].x, _object[i].y, _data);
        }
    }
}

bool Game::CollisionDetect( Position_t* _objectA, uint8_t _sizeA, Position_t* _objectB, uint8_t _sizeB)
{
    for (uint8_t a = 0; a < _sizeA; a++) {
        if (true == IsInScreen(_objectA, a)) {
            for (uint8_t b = 0; b < _sizeB; b++) {
                if (true == IsInScreen(_objectB, b)) {
                    if ((_objectA[a].y == _objectB[b].y) && (_objectA[a].x == _objectB[b].x)) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool Game::CopyObject(Position_t* _dst, uint8_t _dstSize, Position_t* _src, uint8_t _srcSize)
{
    for (uint8_t srcPos = 0; srcPos < _srcSize; srcPos++) {
        if (true == IsInScreen(_src, srcPos)) {
            for (uint8_t dstPos = 0; dstPos < _dstSize; dstPos++) {
                if (false == IsInScreen(_dst, dstPos)) {
                    _dst[dstPos] = _src[srcPos];
                    break;
                }
            }
        }
    }
    return false;
}

Direction_t Game::ReadDirection(void)
{
    Direction_t returnValue = Direction_t::STOPPED;

    if (KB_IsSinglePressed(VK_UP)) {
        returnValue = Direction_t::UP;
    }
    else if (KB_IsSinglePressed(VK_DOWN)) {
        returnValue = Direction_t::DOWN;
    }
    else if (KB_IsSinglePressed(VK_LEFT)) {
        returnValue = Direction_t::LEFT;
    }
    else if (KB_IsSinglePressed(VK_RIGHT)) {
        returnValue = Direction_t::RIGHT;
    }

    return returnValue;
}

void Game::DeleteRow(uint8_t _row)
{
    for (uint8_t i = 0; i < matrixSize; i++) {
        if (matrix[i].y == _row) {
            matrix[i] = { OUT_OF_SCREEN, OUT_OF_SCREEN };
        }
        else if (matrix[i].y < _row) {
            matrix[i].y++;
        }
    }
}
