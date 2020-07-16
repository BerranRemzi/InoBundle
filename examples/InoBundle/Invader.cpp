#include "Invader.h"

Invader::Invader() {
    timer = new Timer(TICK_MEDIUM);
    invaderSpawner = new Timer(200);
    setup();
}

void Invader::setup() {
    for (uint8_t i = 0; i < (SCREEN_WIDTH * SCREEN_HEIGHT); ++i) {
        invader[i] = { OUT_OF_SCREEN, OUT_OF_SCREEN };
    }

    newGame();
}
void Invader::newGame(void) {
    // Start from position (3, 6)
    rocket = { SCREEN_WIDTH / 2, SCREEN_HEIGHT - ROCKET_HEIGHT };

    FillObject(bullet, SCREEN_HEIGHT - ROCKET_HEIGHT, LED_OFF);

    ResetInvader();
}

void Invader::reset() {
    setup();
    ClearScreen();
}

void Invader::ClearScreen(void) {
    // Clear the whole matrix
    for (int x{ 0 }; x < SCREEN_WIDTH; ++x) {
        for (int y{ 0 }; y < SCREEN_HEIGHT; ++y) {
            AB_SetLed(x, y, LED_OFF);
        }
    }
}

void Invader::render() {
    ClearScreen();

    //Draw rocket head
    DrawObject(&rocket, 1);

    //Draw rocket body
    DrawObject(rocketBody, 4);

    //Draw bullets
    DrawObject(bullet, SCREEN_HEIGHT - ROCKET_HEIGHT);

    //Draw invaders
    DrawObject(invader, SCREEN_WIDTH * SCREEN_HEIGHT);

}

void Invader::DrawObject(Position_t* _object, uint8_t _size) {
    for (uint8_t i = 0; i < _size; ++i) {
        if (isInScreen(_object, i)) {
            AB_SetLed(_object[i].x, _object[i].y, LED_ON);
        }
    }
}

void Invader::update() {
    timer->tick();
    invaderSpawner->tick();
    bool ready = timer->isReady();

    switch (state) {
    case GameState::PAUSE:
        break;
    case GameState::GAME_RUN:
    {
        timer->setTick(5);
        state = GameState::GAME_WAIT;

        Direction_t direction = Direction_t::STOPPED;
        bool fire = false;

        if ((KB_IsKeyToggled(VK_RIGHT) && KB_IsKeyDown(VK_RIGHT)) || KB_IsKeyDownLong(VK_RIGHT, 20)) {
            direction = Direction_t::RIGHT;
        }
        else if ((KB_IsKeyToggled(VK_LEFT) && KB_IsKeyDown(VK_LEFT)) || KB_IsKeyDownLong(VK_LEFT, 20)) {
            direction = Direction_t::LEFT;
        }

        if (KB_IsKeyToggled(VK_UP) && KB_IsKeyDown(VK_UP)) {
            fire = true;
        }

        MoveRocket(direction);

        DrawRocket();
        DrawFire(fire);
        DrawInvader(invaderSpawner->isReady());

        CollisionBulletInvader();

        if (CollisionRocketInvader()) {
            state = GameState::ANIM_RUN;
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

        ready = AB_ClearAnimation();
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

void Invader::CollisionBulletInvader() {
    for (uint8_t b = 0; b < SCREEN_HEIGHT - ROCKET_HEIGHT; b++) {
        for (uint8_t i = 0; i < SCREEN_HEIGHT * SCREEN_WIDTH; i++) {
            if ((invader[i].y == bullet[b].y) && (invader[i].x == bullet[b].x)) {
                invader[i] = { OUT_OF_SCREEN, OUT_OF_SCREEN };
                bullet[b] = { OUT_OF_SCREEN, OUT_OF_SCREEN };
            }
        }
    }
}

bool Invader::CollisionRocketInvader() {
 
    for (uint8_t b = 0; b < 4; b++) {
        for (uint8_t i = 0; i < SCREEN_HEIGHT * SCREEN_WIDTH; i++) {
            if ((invader[i].y == rocketBody[b].y) && (invader[i].x == rocketBody[b].x)) {
                return true;
            }
        }
    }
    return false;
}


void Invader::DrawRocket(void) {
    rocketBody[0] = rocket;
    rocketBody[1] = { rocketBody[0].x - 1, rocketBody[0].y + 1 };
    rocketBody[2] = { rocketBody[0].x, rocketBody[0].y + 1 };
    rocketBody[3] = { rocketBody[0].x + 1, rocketBody[0].y + 1 };
}

void Invader::DrawFire(bool _fire) {
    for (uint8_t i = SCREEN_HEIGHT - ROCKET_HEIGHT - 1; i > 0; i--) {
        bullet[i] = bullet[i - 1];
        bullet[i].y--;
    }

    if (_fire) {
        bullet[0] = { rocket.x, rocket.y - 1 };
    }
    else {
        bullet[0] = { OUT_OF_SCREEN, OUT_OF_SCREEN };
    }
}

void Invader::DrawInvader(bool _input) {
    if (true == _input) {
        // Move all invader down
        for (uint8_t i = 0; i < (SCREEN_HEIGHT * SCREEN_WIDTH); i++) {
            if (isInScreen(invader, i)) {
                invader[i].y++;
            }
        }
        uint8_t newInvaders = 0;
        for (uint8_t i = 0; i < (SCREEN_HEIGHT * SCREEN_WIDTH); i++) {
            if (false == isInScreen(invader, i) && newInvaders < SCREEN_WIDTH) {
                invader[i].x = newInvaders;
                invader[i].y = 0;
                newInvaders++;
            }
        }
    }

}

void Invader::ResetInvader(void) {
    //Clear invader position array
    for (uint8_t i = 0; i < SCREEN_HEIGHT * SCREEN_WIDTH; i++) {
        invader[i] = { OUT_OF_SCREEN, OUT_OF_SCREEN };
    }
    //Put invaders to start position
    uint8_t position = 0;
    for (uint8_t x = 0; x < SCREEN_WIDTH; x++) {
        for (uint8_t y = 0; y < INVADER_START_SIZE; y++) {
            invader[position].x = x;
            invader[position].y = y;
            position++;
        }
    }

}

void Invader::MoveRocket(Direction_t _direction) {
    switch (_direction) {
    case Direction_t::RIGHT:
        rocket.x++;
        break;
    case Direction_t::LEFT:
        rocket.x--;
        break;
    default:
        break;
    }

    if (rocket.x >= SCREEN_WIDTH) {
        rocket.x = SCREEN_WIDTH - 1;
    }
    if (rocket.x < 0) {
        rocket.x = 0;
    }
}

bool Invader::isInScreen(Position_t* _object, uint8_t _pos) {
    bool value = false;

    if (_object[_pos].x >= 0 && _object[_pos].x < SCREEN_WIDTH 
        && _object[_pos].y >= 0 && _object[_pos].y < SCREEN_HEIGHT)
    {
        value = true;
    }
    else {
        _object[_pos] = { OUT_OF_SCREEN, OUT_OF_SCREEN };
    }

    return value;
}

void Invader::FillObject(Position_t* _object, uint8_t _size, uint8_t _data) {
    for (uint8_t i = 0; i < _size; ++i) {
        if (isInScreen(_object, i)) {
            AB_SetLed(_object[i].x, _object[i].y, _data);
        }
    }
}

Sound_t Invader::playSound() {
    if (sound > Sound_t::SNAKE_SILENCE) {
        sound = Sound_t::SNAKE_SILENCE;
    }

    return sound;
}
