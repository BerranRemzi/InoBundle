#include "Tetris.h"

Tetris::Tetris(void)
{
    timer = new Timer(5);
    spawner = new Timer(100);
    setup();
}

void Tetris::update()
{
    timer->tick();
    spawner->tick();

    bool ready = timer->isReady();

    switch (state) {
    case GameState::PAUSE:
        break;
    case GameState::GAME_RUN:
    {
        timer->setTick(5);
        state = GameState::GAME_WAIT;

        position = ReadDirection(&position);

        if (spawner->isReady()) {
            // Move block down
            position.y++;
        }

        if (KB_IsSinglePressed(VK_A)) {
            rotation++;
            rotation = rotation % 4;
        }
        if (KB_IsSinglePressed(VK_X)) {
            type++;
            type = type % 7;
        }

        DrawBlock(type, rotation, position);
        //MoveRocket(direction);

        //DrawRocket();
        //DrawFire(fire);
        //DrawInvader(invaderSpawner->isReady());

        //CollisionBulletInvader();

        //if (CollisionRocketInvader()) {
        //	state = GameState::ANIM_RUN;
        //}

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

void Tetris::setup(void)
{
    for (uint8_t i = 0; i < (SCREEN_WIDTH * SCREEN_HEIGHT); ++i) {
        storedBricks[i] = { OUT_OF_SCREEN, OUT_OF_SCREEN };
    }
}

void Tetris::reset(void)
{
    setup();
    ClearScreen();
}

void Tetris::render(void)
{
    ClearScreen();

    //Draw brick
    DrawObject(block, BRICK_ARRAY_SIZE);

    //Draw stored bricks
    DrawObject(storedBricks, SCREEN_HEIGHT * SCREEN_WIDTH);
}

void Tetris::HandleAction(Direction_t _dir)
{
}

void Tetris::DrawBlock(uint8_t _type, uint8_t _rotation, Position_t _pos)
{
    uint8_t pos = 0;
    for (uint8_t x = 0u; x < BLOCK_WIDTH; x++) {
        for (uint8_t y = 0u; y < BLOCK_HEIGHT; y++) {
            if (((blocksArray[_type][_rotation] >> pos) & 1u) == true) {
                block[pos].x = _pos.x + x;
                block[pos].y = _pos.y + y;
            }
            else {
                block[pos] = { OUT_OF_SCREEN, OUT_OF_SCREEN };
            }
            pos++;
        }
    }
}

Position_t Tetris::ReadDirection(Position_t* _pos)
{
    if (KB_IsSinglePressed(VK_UP)) {
        _pos->y--;
    }
    else if (KB_IsSinglePressed(VK_DOWN)) {
        _pos->y++;
    }
    else if (KB_IsSinglePressed(VK_LEFT)) {
        _pos->x--;
    }
    else if (KB_IsSinglePressed(VK_RIGHT)) {
        _pos->x++;
    }

    return *_pos;
}

bool Tetris::CollisionStoredBlock(void)
{
    return false;
}

bool Tetris::ColiisionBoardLimit(void)
{
    return false;
}