#include "Snake.h"

void Start(Snake_t* snake)
{
    // Clear position array
    for (uint8_t i = 0; i < SNAKE_MAX_SIZE; i++) {
        snake->position[i].X = OUT_OF_SCREEN;
        snake->position[i].Y = OUT_OF_SCREEN;
    }

    // Snake's tail position (0, 3)
    snake->position[0].X = 0;
    snake->position[0].Y = 3;

    // Snake's head posiiton (1, 3)
    snake->position[1].X = 1;
    snake->position[1].Y = 3;

    // Initially, move to the right
    snake->headDir.X = RIGHT_MOVE;
    snake->headDir.Y = NO_MOVE;

    snake->size = 2;

    // Clear the screen then draw tail & head of the snake
    Clear();
    Pixel(snake->position[0].X, snake->position[0].Y, HIGH);
    Pixel(snake->position[1].X, snake->position[1].Y, HIGH);

    GenerateFood(snake);
}

void Update(Snake_t* snake)
{
    MoveSnake(snake);
}

void MoveSnake(Snake_t* snake)
{
    if (GetButton(Button_t::UP_BTN) && snake->headDir.Y != DOWN_MOVE)
    {
        snake->headDir.Y = UP_MOVE;
        snake->headDir.X = NO_MOVE;
    }
    if (GetButton(Button_t::DOWN_BTN) && snake->headDir.Y != UP_MOVE)
    {
        snake->headDir.Y = DOWN_MOVE;
        snake->headDir.X = NO_MOVE;
    }
    if (GetButton(Button_t::LEFT_BTN) && snake->headDir.X != RIGHT_MOVE)
    {
        snake->headDir.X = LEFT_MOVE;
        snake->headDir.Y = NO_MOVE;
    }
    if (GetButton(Button_t::RIGHT_BTN) && snake->headDir.X != LEFT_MOVE)
    {
        snake->headDir.X = RIGHT_MOVE;
        snake->headDir.Y = NO_MOVE;
    }

    Position_t head = snake->position[snake->size - 1];
    Position_t tail = snake->position[0];

    // Clamp snake head position
    head.X += snake->headDir.X;
    head.Y += snake->headDir.Y;
    MirrorClamp(&head.X, 0, SCREEN_WIDTH);
    MirrorClamp(&head.Y, 0, SCREEN_HEIGHT);

    if (head.X == snake->food.X && head.Y == snake->food.Y) {
        ConsumeFood(snake);
    }
    else {
        // If head is colliding with any other part of the snake, re-start the game
        if (GetPixel(head.X, head.Y)) {
            Start(snake);
        }
        else {
            Pixel(head.X, head.Y, HIGH);
            Pixel(tail.X, tail.Y, LOW);

            for (uint8_t i = 0; i < snake->size - 1; i++){
                snake->position[i] = snake->position[i + 1];
            }
        }

        // Update snake's head position
        snake->position[snake->size - 1] = head;
    }
}

void GenerateFood(Snake_t* snake)
{
    snake->food.X = (uint8_t)random(SCREEN_WIDTH);
    snake->food.Y = (uint8_t)random(SCREEN_HEIGHT);

    // Draw the food
    Pixel(snake->food.X, snake->food.Y, HIGH);
}

void ConsumeFood(Snake_t* snake)
{
    snake->size++;

    // Set new head's position to the food's position
    snake->position[snake->size - 1] = snake->food;

    GenerateFood(snake);
}

void MirrorClamp(int8_t* value, int8_t min, int8_t max) {
    if (*value < min) *value += max;
    else if (*value >= max) *value %= max;
}