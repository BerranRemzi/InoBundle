#include "Joystick.h"

Joystick::Joystick()
{
    Mouse.begin();
    //Keyboard.begin();
}

void Joystick::update()
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

    /*
    // use the pushbuttons to control the keyboard:
    if (digitalRead(upButton) == HIGH)
    {
        Keyboard.write('u');
    }
    if (digitalRead(downButton) == HIGH)
    {
        Keyboard.write('d');
    }
    if (digitalRead(leftButton) == HIGH)
    {
        Keyboard.write('l');
    }
    if (digitalRead(rightButton) == HIGH)
    {
        Keyboard.write('r');
    }
    if (digitalRead(mouseButton) == HIGH)
    {
        Keyboard.write('m');
    }
    */
}