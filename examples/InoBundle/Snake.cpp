#include "Snake.h"

Snake::Snake() {
    timer = new Timer(TICK_MEDIUM);
    setup();
}

void Snake::setup(void) {
    for (int i{ 0 }; i < SCREEN_WIDTH * SCREEN_HEIGHT; ++i) {
        matrix[i] = { OUT_OF_SCREEN, OUT_OF_SCREEN };
    }

    newGame();
}
void Snake::newGame(void) {
    // Start from position (0, 3) and direction is "right"
    currentHead.x = 0;
    currentHead.y = 3;

    size = SNAKE_START_SIZE;

    lastDirection = Direction_t::RIGHT;
    food = { OUT_OF_SCREEN, OUT_OF_SCREEN };
    isFoodGenerated = false;
    generateFood();
}

void Snake::reset(void) {
    setup();

    if (size > 5) {
        speed++;
    }
    else if (speed > 0) {
        speed--;
    }

    sound = Sound_t::SNAKE_DEAD;
    // Clear the whole matrix
    for (uint8_t x = 0; x < SCREEN_WIDTH; ++x) {
        for (uint8_t y = 0; y < SCREEN_HEIGHT; ++y) {
            SetLed(x, y, LED_OFF);
        }
    }
}

const void Snake::render(void) {
    // Clear old snake head
    SetLed(previousHead.x, previousHead.y, LED_OFF);

    // Clear old last part of the snake matrix if exists
    if (size > 0) {
        SetLed(tail.x, tail.y, LED_OFF);
    }

    // Print snake matrix
    DrawObject(matrix, SCREEN_WIDTH * SCREEN_HEIGHT);

    // Print snake head
    SetLed(currentHead.x, currentHead.y, LED_ON);

    // Print food
    SetLed(food.x, food.y, LED_ON);
}

void Snake::update() {
    timer->tick();
    bool ready = timer->isReady();

    switch (state) {
    case GameState::PAUSE:
        break;
    case GameState::GAME_RUN:
    {
        timer->setTick(TICK_MEDIUM);
        state = GameState::GAME_WAIT;

        Direction_t currentDirection = getLastDirection();

        /* Prevent going to opposite direction */
        Direction_t direction = GetValidDirection(currentDirection);

        MoveSnake(direction);

        // If head is collided with food
        if (food.x == currentHead.x && food.y == currentHead.y) {
            isFoodGenerated = false;
            generateFood();
            extendSnake();
            sound = Sound_t::SNAKE_SIZE_UP;
        }
        else {
            moveBody();
        }

        for (int i = 0; i < size; ++i) {
            // If head is collided with matrix
            if (currentHead.x == matrix[i].x && currentHead.y == matrix[i].y) {
                state = GameState::ANIM_RUN;
            }
        }
        // Print to matrix
        render();
    }
    break;
    case GameState::GAME_WAIT:
        if (ready)
            state = GameState::GAME_RUN;
        break;
    case GameState::ANIM_RUN:
        timer->setTick(TICK_ANIM);
        state = GameState::ANIM_WAIT;

        ready = ClearAnimation();
        if (ready) {
            state = GameState::RESTART;
        }
        break;
    case GameState::ANIM_WAIT:
        if (ready)
            state = GameState::ANIM_RUN;
        break;
    case GameState::BLINK:
        break;
    case GameState::RESTART:
        reset();
        render();
        state = GameState::GAME_RUN;
        break;
    default:
        break;
    }
}
const Direction_t Snake::GetValidDirection(Direction_t _currentDirection) {
    static Direction_t direction = Direction_t::RIGHT;

    if (_currentDirection != Direction_t::STOPPED) {
        if (Direction_t::UP != direction && Direction_t::DOWN == _currentDirection) {
            direction = _currentDirection;
        }
        if (Direction_t::DOWN != direction && Direction_t::UP == _currentDirection) {
            direction = _currentDirection;
        }
        if (Direction_t::RIGHT != direction && Direction_t::LEFT == _currentDirection) {
            direction = _currentDirection;
        }
        if (Direction_t::LEFT != direction && Direction_t::RIGHT == _currentDirection) {
            direction = _currentDirection;
        }
    }
    return direction;
}

void Snake::generateFood(void) {
    if (isFoodGenerated == true) {
        return;
    }
    // TODO: Fix food appearing inside of matrix or head,
    // if it appears inside the head, size doesn't grow
    putFood();
    for (int i = 0; i < TOTAL_FOOD_TRY_COUNT; ++i) {
        for (int j = 0; j < SCREEN_WIDTH * SCREEN_HEIGHT; ++j) {
            if (food.x == matrix[j].x && food.y == matrix[j].y) {
                putFood();
            }
            else if (currentHead.x == matrix[j].x && currentHead.y == matrix[j].y) {
                putFood();
            }
            else {
                isFoodGenerated = true;
                break;
            }
        }
    }
}

void Snake::putFood(void) {
    food.x = (uint8_t)random(8);
    food.y = (uint8_t)random(8);
}

void Snake::extendSnake(void) {
    matrix[size].x = previousHead.x;
    matrix[size].y = previousHead.y;

    ++size;
}

void Snake::MoveSnake(Direction_t _newDirection) {
    // If snake is going to move
    previousHead.x = currentHead.x;	// Set old head x to current head x
    previousHead.y = currentHead.y;	// Set old head y to current head y

    sound = Sound_t::SNAKE_MOVE;

    if (_newDirection == Direction_t::UP) {
        --currentHead.y;
        // If snake head is out of the matrix, teleport it to mirrored position
        if (currentHead.y < 0)
            currentHead.y = SCREEN_HEIGHT - 1;
    }
    else if (_newDirection == Direction_t::DOWN) {
        ++currentHead.y;
        // If snake head is out of the matrix, teleport it to mirrored position
        if (currentHead.y == SCREEN_HEIGHT)
            currentHead.y = 0;
    }
    else if (_newDirection == Direction_t::RIGHT) {
        ++currentHead.x;
        // If snake head is out of the matrix, teleport it to mirrored position
        if (currentHead.x == SCREEN_WIDTH)
            currentHead.x = 0;
    }
    else if (_newDirection == Direction_t::LEFT) {
        --currentHead.x;
        // If snake head is out of the matrix, teleport it to mirrored position
        if (currentHead.x < 0)
            currentHead.x = SCREEN_WIDTH - 1;
    }
}

Direction_t Snake::getLastDirection(void) {
    if (KB_IsKeyPressed(VK_UP) && Direction_t::UP != lastDirection) {
        lastDirection = Direction_t::UP;
    }

    if (KB_IsKeyPressed(VK_DOWN) && Direction_t::DOWN != lastDirection) {
        lastDirection = Direction_t::DOWN;
    }

    if (KB_IsKeyPressed(VK_RIGHT) && Direction_t::RIGHT != lastDirection) {
        lastDirection = Direction_t::RIGHT;
    }

    if (KB_IsKeyPressed(VK_LEFT) && Direction_t::LEFT != lastDirection) {
        lastDirection = Direction_t::LEFT;
    }
    KB_Reset();
    return lastDirection;
}

void Snake::moveBody(void) {
    if (size > 0) {
        tail.x = matrix[0].x;
        tail.y = matrix[0].y;

        for (int i{ 0 }; i < size - 1; ++i) {
            matrix[i].x = matrix[i + 1].x;
            matrix[i].y = matrix[i + 1].y;
        }

        matrix[size - 1].x = previousHead.x;
        matrix[size - 1].y = previousHead.y;
    }
}

Sound_t Snake::playSound(void) {
    if (sound > Sound_t::SNAKE_SILENCE) {
        sound = Sound_t::SNAKE_SILENCE;
    }

    return sound;
}