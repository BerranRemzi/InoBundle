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

        Direction_t dir = Direction_t::STOPPED;
        bool fire = false;

        if ((KB_IsKeyToggled(VK_RIGHT) && KB_IsKeyDown(VK_RIGHT)) || KB_IsKeyDownLong(VK_RIGHT, 20)) {
            dir = Direction_t::RIGHT;
        }
        else if ((KB_IsKeyToggled(VK_LEFT) && KB_IsKeyDown(VK_LEFT)) || KB_IsKeyDownLong(VK_LEFT, 20)) {
            dir = Direction_t::LEFT;
        }

        if (KB_IsKeyToggled(VK_A) && KB_IsKeyDown(VK_A)) {
            fire = true;
        }

        MoveRocket(dir);

        DrawRocket();
        DrawFire(fire);
        DrawInvader(invaderSpawner->isReady());

        CollisionBulletInvader();

        if (CollisionDetect(rocketBody, ROCHET_BODY_SIZE, invader, INVADER_MAX_COUNT)) {
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
            if (IsInScreen(invader, i)) {
                invader[i].y++;
            }
        }
        uint8_t newInvaders = 0;
        for (uint8_t i = 0; i < (SCREEN_HEIGHT * SCREEN_WIDTH); i++) {
            if (false == IsInScreen(invader, i) && newInvaders < SCREEN_WIDTH) {
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

Sound_t Invader::playSound() {
    if (sound > Sound_t::SNAKE_SILENCE) {
        sound = Sound_t::SNAKE_SILENCE;
    }

    return sound;
}