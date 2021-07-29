#include "../InoBundle.h"

#define SNAKE_MAX_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT)
#define SNAKE_START_SIZE 2

struct Snake_t
{
    int8_t positionX[SNAKE_MAX_SIZE];
    int8_t positionY[SNAKE_MAX_SIZE];

    int8_t movementX; 
    int8_t movementY; 

    uint8_t foodX;
    uint8_t foodY;

    uint8_t size;
};

void Start(Snake_t* snake);
void Update(Snake_t* snake);
void MoveSnake(Snake_t* snake);
void GenerateFood(Snake_t* snake);
void ConsumeFood(Snake_t* snake);
void MirrorClamp(int8_t* value, int8_t min, int8_t max);