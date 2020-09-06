#include "Arduino.h"

#include "ArcadeBundle.h"
#include "Snake.h"
#include "Brick.h"
#include "Invader.h"
#include "Flappy.h"
#include "Tetris.h"
#include "Demo.h"
#include "Calculator.h"
#include "Joystick.h"

#include "Keyboard.h"
#include "xOS.h"

enum Game_t
{
    GAME_SNAKE,
    GAME_BRICK,
    GAME_INVADER,
    GAME_FLAPPY,
    GAME_TETRIS,
    //GAME_SQUARE,  //not implemented yet
    GAME_DEMO,
    GAME_CALCULATOR,
    GAME_JOYSTICK,
    GAME_COUNT
};

enum TaskNames
{
    TASK_GAME,
    TASK_SCREEN,
    TASK_KEYBOARD
};

void Task_Keyboard(void);
void Task_Game(void);
void Task_Screen(void);

Task_t TaskStruct[4];

Game *game = new Joystick();

uint8_t currGame = GAME_JOYSTICK;

void setup()
{
    //Initialize serial and wait for port to open:
    Serial.begin(115200);
    while (!Serial)
    {
        ; // wait for serial port to connect. Needed for native USB
    }

    AB_HAL_AnalogPrescaler(8);
    game->Setup();

    xInit(TaskStruct); //Struct with function parameters
    xTaskCreate(&Task_Screen, 1);
    xTaskCreate(&Task_Keyboard, 10);
    xTaskCreate(&Task_Game, 10);
    xTaskCreate(&Task_Debug, 10);
}

void loop()
{
    xLoop(); //xOS task
}

void Task_Game(void)
{
    TASK_ENTER(TASK_GAME);
    game->update();
    TASK_LEAVE(TASK_GAME);
}

void Task_Screen(void)
{
    TASK_ENTER(TASK_SCREEN);
    game->UpdateScreen();
    TASK_LEAVE(TASK_SCREEN);
}

void Task_Keyboard(void)
{
    TASK_ENTER(TASK_KEYBOARD);
    KB_ReadAll();
    if (KB_IsKeyDownLong(VK_Y, 100))
    { /* wait for 100ticks = 1000ms */
        game->ClearDisplay();
        free(game);
        switch (++currGame)
        {
        case GAME_SNAKE:
            game = new Snake();
            break;
        case GAME_BRICK:
            game = new Brick();
            break;
        case GAME_INVADER:
            game = new Invader();
            break;
        case GAME_FLAPPY:
            game = new Flappy();
            break;
        case GAME_TETRIS:
            game = new Tetris();
            break;
        case GAME_DEMO:
            game = new Demo();
            break;
        case GAME_CALCULATOR:
            game = new Calculator();
            break;
        case GAME_JOYSTICK:
            game = new Joystick();
            break;
        default:
            game = new Snake();
            currGame = 0;
            break;
        }
    }
    TASK_LEAVE(TASK_KEYBOARD);
}

void Task_Debug(void)
{
    TASK_PUT_TOTAL();
    TASK_PRINT_ALL();
}