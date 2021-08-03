#include "../InoBundle.h"

#define SNAKE_MAX_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT)
#define SNAKE_START_SIZE 3


#define NO_MOVE     0
#define RIGHT_MOVE  1
#define LEFT_MOVE   -1
#define UP_MOVE     -1
#define DOWN_MOVE   1

#define OUT_OF_SCREEN -1

typedef struct{
    int8_t X;
    int8_t Y;
}Position_t;

typedef struct{
    int8_t X;
    int8_t Y;
}Movement_t;

typedef struct 
{
    Position_t position[SNAKE_MAX_SIZE];

    Movement_t headDir;
    Movement_t tailDir;

    Position_t food;

    uint8_t size;
}Snake_t;

void Start(Snake_t* snake);
void Update(Snake_t* snake);
void MoveSnake(Snake_t* snake);
void GenerateFood(Snake_t* snake);
void ConsumeFood(Snake_t* snake);
void MirrorClamp(int8_t* value, int8_t min, int8_t max);