#include "../InoBundle.h"

struct Snake_t
{
    int snakeX = 0;
    int snakeY = 0;
    int size = 1;
};

void Start();
void MoveSnake();
void GenerateFood();
void ConsumeFood();