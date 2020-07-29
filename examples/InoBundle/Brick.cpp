#include "Brick.h"

Brick::Brick() {
    timer = new Timer(0);
    reset();
}

void Brick::reset() {
    for (int i{ 0 }; i < 8; ++i)
        screen[i] = 0;

    brickHeight = SCREEN_HEIGHT - 1;
    timer->setTick(TICK_FAST);
    bricksOnScreen = 0;
    direction = Direction_t::RIGHT;
    brickLength = 3;
}

void Brick::update() {
    timer->tick();
    bool ready = timer->isReady();
    bool isDetected = false;

    switch (state) {
    case GameState::PAUSE:
        break;
    case GameState::GAME_RUN:
        state = GameState::GAME_WAIT;

        if ((KB_IsKeyToggled(VK_DOWN) && KB_IsKeyDown(VK_DOWN))
            || (KB_IsKeyToggled(VK_A) && KB_IsKeyDown(VK_A))) {
            isDetected = true;
        }

        /* Place brick based on gameplay state */
        if (BrickState::GOTO_RESTART == PlaceBrick(isDetected)) {
            state = GameState::ANIM_RUN;
        }
        /* Move bricks to right and left directions */
        MoveBrick();

        render();
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
    }
}

BrickState Brick::PlaceBrick(bool _collision) {
    switch (brickState) {
    case BrickState::MOVING:
        if (_collision) {
            brickState = BrickState::CHECK_NEW_LINE;
        }
        break;
    case BrickState::CHECK_NEW_LINE:

        if (brickHeight >= 0) {
            if (brickHeight < 7) {
                /* trim brick based on previous row */
                screen[brickHeight] &= screen[brickHeight + 1];
                /* get length of brick after trimming */
                brickLength = GetBrickLength(brickHeight);
            }

            /* switch to next row */
            MoveToNextRow();

            /* change brick direction to opposite side */
            GotoOppositeDirection();
        }

        if (brickHeight == OUT_OF_SCREEN || brickLength == 0) {
            brickState = BrickState::GOTO_RESTART;
        }
        else {
            brickState = BrickState::MOVING;
        }

        break;
    case BrickState::GOTO_RESTART:
        brickState = BrickState::MOVING;
        break;
    default:
        break;
    }
    return brickState;
}

void Brick::MoveToNextRow() {
    if (brickHeight > OUT_OF_SCREEN) {
        brickHeight--;
    }
    bricksOnScreen = 0;
}

void Brick::GotoOppositeDirection() {
    if (direction == Direction_t::RIGHT) {
        direction = Direction_t::LEFT;
    }
    else {
        direction = Direction_t::RIGHT;
    }
}

uint8_t Brick::GetBrickLength(uint8_t _row) {
    uint8_t counter = 0;
    for (uint8_t i = 0; i < 8; i++) {
        if (checkBit(screen[brickHeight], i) == true) {
            counter++;
        }
    }

    // If brick is at top, the next brick length must be 0
    if (_row == 0) {
        counter = 0;
    }
    return counter;
}

void Brick::render() {
    for (uint8_t y = 0; y < 8; y++) {
        for (uint8_t x = 0; x < 8; x++) {
            if (checkBit(screen[y], x)) {
                SetLed(x, y, LED_ON);
            }
            else {
                SetLed(x, y, LED_OFF);
            }
        }
    }
}

void Brick::MoveBrick() {
    if (direction == Direction_t::RIGHT) {
        screen[brickHeight] >>= 1;
        if (brickLength - bricksOnScreen > 0) {
            screen[brickHeight] |= 0x80;
            bricksOnScreen++;
        }
        if (screen[brickHeight] == 0x00) {
            direction = Direction_t::LEFT;
            bricksOnScreen = 0;
        }
    }
    else if (direction == Direction_t::LEFT) {
        screen[brickHeight] <<= 1;
        if (brickLength - bricksOnScreen > 0) {
            screen[brickHeight] |= 0x01;
            bricksOnScreen++;
        }
        if (screen[brickHeight] == 0x00) {
            direction = Direction_t::RIGHT;
            bricksOnScreen = 0;
        }
    }
}