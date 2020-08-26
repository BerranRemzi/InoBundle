#include "Arduino.h"

#include "ArcadeBundle.h"
#include "Snake.h"
#include "Brick.h"
#include "Invader.h"
#include "Flappy.h"
#include "Tetris.h"
#include "Demo.h"

#include "Keyboard.h"
#include "xOS.h"

enum Game_t
{
    GAME_SNAKE,
    GAME_BRICK,
    GAME_INVADER,
    GAME_FLAPPY,
    GAME_TETRIS, //not implemented yet
    //GAME_SQUARE,  //not implemented yet
    GAME_DEMO,
    GAME_COUNT
};

void Task_Keyboard(void);
void Task_Game(void);
void Task_Screen(void);

Task_t TaskStruct[4];

Game *game = new Tetris();
//Brick brick;
//Invader invader;		//not implemented yet
//Tetris tetris;		//not implemented yet
//Square square;		//not implemented yet
//Demo demo;			//not implemented yet

uint8_t currGame = GAME_INVADER;

void setup()
{
    game->Setup();

    xInit(TaskStruct); //Struct with function parameters
    xTaskCreate(&Task_Keyboard, 10);
    xTaskCreate(&Task_Game, 10);
    xTaskCreate(&Task_Screen, 10);
}

void loop()
{
    xLoop(); //xOS task
}

void Task_Game(void)
{
    game->update();
}

void Task_Screen(void)
{
    game->UpdateScreen();
}

void Task_Keyboard(void)
{
    KB_ReadAll();
    if (KB_IsKeyDownLong(VK_UP, 200) || KB_IsKeyDownLong(VK_Y, 100))
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
        default:
            game = new Snake();
            currGame = 0;
            break;
        }
    }
}