#include "Arduino.h"
#include "InoBundle.h"
#include <TimerOne.h>

const uint8_t row[] = {4, 3, 8, 12, A4, 9, A3, 5};
const uint8_t col[] = {2, A2, 13, 6, 10, 11, A5, 7};

volatile uint16_t screen[SCREEN_HEIGHT];

void Init(void)
{
    for (int y = 0; y < SCREEN_HEIGHT; y++)
    {
        pinMode(row[y], OUTPUT);
    }
    for (int x = 0; x < SCREEN_WIDTH; x++)
    {
        pinMode(col[x], OUTPUT);
    }

    UpdateScreen();

    Timer1.initialize(UPDATE_PERIOD_US);
    Timer1.attachInterrupt(UpdateScreen);
}

void UpdateScreen(void)
{
    static uint8_t y = 0;
    digitalWrite(row[y], ROW_OFF);

    y++;
    if (y >= SCREEN_HEIGHT)
    {
        y = 0;
    }

    for (uint8_t x = 0; x < SCREEN_WIDTH; x++)
    {
        if (COLUMN_ON)
        {
            digitalWrite(col[x], !(screen[y] >> x) & 1U);
        }
        else
        {
            digitalWrite(col[x], (screen[y] >> x) & 1U);
        }
    }
    digitalWrite(row[y], ROW_ON);
}

void Pixel(int x, int y, bool state)
{
    if (state)
    {
        screen[y] |= 1UL << x;
    }
    else
    {
        screen[y] &= ~(1UL << x);
    }
}

void WriteRow(int y, uint16_t value)
{
    screen[y] = value;
}

void WriteCol(int x, uint16_t value)
{
    for (int y = 0; y < SCREEN_HEIGHT; y++)
    {
        Pixel(x, y, (value >> y) & 1U);
    }
}

void Clear(void)
{
    for (int y = 0; y < SCREEN_HEIGHT; y++)
    {
        screen[y] = 0x00;
    }
}