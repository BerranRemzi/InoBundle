#include "../InoBundle.h"

struct Snake_t
{
    uint8_t snakeX;
    uint8_t snakeY;
    uint8_t foodX;
    uint8_t foodY;
    uint8_t size;
};

void Start();
void Update();
void MoveSnake();
void GenerateFood();
void ConsumeFood();