#include "Snake.h"

void Start(Snake_t* snake)
{
    // Clear position array
    for (uint8_t i = 0; i < SNAKE_MAX_SIZE; i++) {
        snake->positionX[i] = -1;
        snake->positionY[i] = -1;
    }

    // Snake's tail position (0, 3)
    snake->positionX[0] = 0;
    snake->positionY[0] = 3;

    // Snake's head posiiton (1, 3)
    snake->positionX[1] = 1;
    snake->positionY[1] = 3;

    // Initially, move to the right
    snake->movementX = 1;
    snake->movementY = 0;

    snake->size = SNAKE_START_SIZE;

    // Clear the screen then draw tail & head of the snake
    Clear();
    Pixel(snake->positionX[0], snake->positionY[0], HIGH);
    Pixel(snake->positionX[1], snake->positionY[1], HIGH);

    GenerateFood(snake);
}

void Update(Snake_t* snake)
{
    MoveSnake(snake);
}

void MoveSnake(Snake_t* snake)
{
    if (GetButton(Button_t::UP_BTN) && snake->movementY != 1)
    {
        snake->movementY = -1;
        snake->movementX = 0;
    }
    if (GetButton(Button_t::DOWN_BTN) && snake->movementY != -1)
    {
        snake->movementY = 1;
        snake->movementX = 0;
    }
    if (GetButton(Button_t::LEFT_BTN) && snake->movementX != 1)
    {
        snake->movementX = -1;
        snake->movementY = 0;
    }
    if (GetButton(Button_t::RIGHT_BTN) && snake->movementX != -1)
    {
        snake->movementX = 1;
        snake->movementY = 0;
    }

    int8_t headX = snake->positionX[snake->size - 1];
    int8_t headY = snake->positionY[snake->size - 1];
    int8_t tailX = snake->positionX[0];
    int8_t tailY = snake->positionY[0];

    // Clamp snake head position
    headX += snake->movementX;
    headY += snake->movementY;
    MirrorClamp(&headX, 0, SCREEN_WIDTH);
    MirrorClamp(&headY, 0, SCREEN_HEIGHT);

    if (headX == snake->foodX && headY == snake->foodY) {
        ConsumeFood(snake);
    }
    else {
        // If head is colliding with any other part of the snake, re-start the game
        if (GetPixel(headX, headY)) {
            Start(snake);
        }
        else {
            Pixel(headX, headY, HIGH);
            Pixel(tailX, tailY, LOW);

            for (uint8_t i = 0; i < snake->size - 1; i++){
                snake->positionX[i] = snake->positionX[i + 1];
                snake->positionY[i] = snake->positionY[i + 1];
            }
        }

        // Update snake's head position
        snake->positionX[snake->size - 1] = headX;
        snake->positionY[snake->size - 1] = headY;
    }
}

void GenerateFood(Snake_t* snake)
{
    snake->foodX = (uint8_t)random(SCREEN_WIDTH);
    snake->foodY = (uint8_t)random(SCREEN_HEIGHT);

    // Draw the food
    Pixel(snake->foodX, snake->foodY, HIGH);
}

void ConsumeFood(Snake_t* snake)
{
    snake->size++;

    // Set new head's position to the food's position
    snake->positionX[snake->size - 1] = snake->foodX;
    snake->positionY[snake->size - 1] = snake->foodY;

    GenerateFood(snake);
}

void MirrorClamp(int8_t* value, int8_t min, int8_t max) {
    if (*value < min) *value += max;
    else if (*value >= max) *value %= max;
}