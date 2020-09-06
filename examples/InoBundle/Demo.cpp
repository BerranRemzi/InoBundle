#include "Demo.h"

void Demo::update()
{
    static bool fill = true;
    static uint8_t data[8];

    int8_t counter = 0;

    for (uint8_t t = 0; t < 1; t++)
    {
        uint8_t randomX = random(8);
        uint8_t randomY = random(8);

        bool led = (data[randomY] & 1 << randomX);

        if (false == led && true == fill)
        {
            data[randomY] |= 1 << randomX;
            SetRow(randomY, data[randomY]);
            break;
        }

        if (true == led && false == fill)
        {
            data[randomY] ^= 1 << randomX;
            SetRow(randomY, data[randomY]);
            break;
        }
    }
    for (uint8_t y = 0; y < 8; y++)
    {
        if (0xFF == data[y])
        {
            counter++;
        }
        if (0x00 == data[y])
        {
            counter--;
        }
    }

    if (counter > 7)
    {
        fill = false;
    }
    if (counter < -7)
    {
        fill = true;
    }
}