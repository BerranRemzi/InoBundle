#include "Joystick.h"

Joystick::Joystick()
{
    Mouse.begin();
    Keyboard.begin();
}

void Joystick::update()
{
    if (mode)
    {
        if (KB_IsKeyDown(VK_UP))
        {
            Mouse.move(0, -mouseMoveSpeed++ / 5);
        }
        else if (KB_IsKeyDown(VK_LEFT))
        {
            Mouse.move(-mouseMoveSpeed++ / 5, 0);
        }
        else if (KB_IsKeyDown(VK_RIGHT))
        {
            Mouse.move(mouseMoveSpeed++ / 5, 0);
        }
        else if (KB_IsKeyDown(VK_DOWN))
        {
            Mouse.move(0, mouseMoveSpeed++ / 5);
        }
        else
        {
            mouseMoveSpeed = 5u;
        }

        if (KB_IsKeyDown(VK_Y))
        {
            Mouse.move(0, 0, 1);
        }
        else if (KB_IsKeyDown(VK_A))
        {
            Mouse.move(0, 0, -1);
        }

        if (KB_IsKeyDown(VK_X))
        {
            Mouse.press(MOUSE_LEFT);
        }
        else
        {
            Mouse.release(MOUSE_LEFT);
        }
        if (KB_IsKeyDown(VK_B))
        {
            Mouse.click(MOUSE_RIGHT);
        }
        else
        {
            Mouse.release(MOUSE_RIGHT);
        }
    }
    else
    {
        if (KB_IsKeyDown(VK_UP) || KB_IsKeyDown(VK_B))
        {
            Keyboard.press('w');
        }
        else
        {
            Keyboard.release('w');
        }

        if (KB_IsKeyDown(VK_LEFT))
        {
            Keyboard.press('a');
        }
        else
        {
            Keyboard.release('a');
        }

        if (KB_IsKeyDown(VK_DOWN))
        {
            Keyboard.press('s');
        }
        else
        {
            Keyboard.release('s');
        }

        if (KB_IsKeyDown(VK_RIGHT))
        {
            Keyboard.press('d');
        }
        else
        {
            Keyboard.release('d');
        }

        if (KB_IsKeyDown(VK_A))
        {
            Keyboard.press(KEY_LEFT_SHIFT);
        }
        else
        {
            Keyboard.release(KEY_LEFT_SHIFT);
        }
    }

    // Change mode
    if (KB_IsKeyDownLong(VK_A, 200))
    {
        mode = !mode;
    }
}