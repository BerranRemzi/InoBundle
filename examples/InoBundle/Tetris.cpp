#include "Tetris.h"

Tetris::Tetris(void)
{
    timer = new Timer(5);
    dropTimer = new Timer(100);
    setup();
}

void Tetris::update()
{
    timer->tick();
    dropTimer->tick();

    bool ready = timer->isReady();
    static bool isReadyForNewBlock = false;

    switch (state) {
    case GameState::PAUSE:
        break;
    case GameState::GAME_RUN:
    {
        timer->setTick(5);
        state = GameState::GAME_WAIT;

        Direction_t dir = ReadDirection();

        position = GetNewBlockPosition(&position, dir);



        if (dropTimer->isReady()) {
            // Move block down
            position.y++;
            TryToDropPiece();
        }

        if (isReadyForNewBlock) {
            position.y = -3;
        }

        if (KB_IsSinglePressed(VK_A)) {
            rotation++;
            rotation = rotation % 4;
        }
        if (KB_IsSinglePressed(VK_X)) {
            type++;
            type = type % 7;
        }

        //MovoBlockToScreen();
        isReadyForNewBlock = TryToMoveBlock(type, rotation, &position);

        /* Draw currenct block */
        DrawBlock(type, rotation, position);

        if (CollisionDetect(grid, SCREEN_HEIGHT * SCREEN_WIDTH, block, BRICK_ARRAY_SIZE)) {
            isReadyForNewBlock = true;
        }

        if (isReadyForNewBlock || KB_IsSinglePressed(VK_B)) {
            //TryToMoveBlock(type, rotation, &position);
            CopyObject(grid, SCREEN_HEIGHT * SCREEN_WIDTH, block, BRICK_ARRAY_SIZE);
            
        }
        RemoveFullRows();
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
void Tetris::RemoveFullRows(void)
{
    for (uint8_t r = 0; r < SCREEN_HEIGHT; r++) {
        uint8_t c = 0;
        for (uint8_t g = 0; g < gridSize; g++) {
            if (grid[g].y == r) {
                c++;
            }
        }
        if (c == SCREEN_WIDTH) {
            DeleteRow(r);
        }
    }
}
void Tetris::TryToDropPiece(void)
{
}
bool Tetris::TryToMoveBlock(uint8_t _type, uint8_t _rotation, Position_t* _pos) {
    Position_t correction = { 0, 0 };
    Position_t tempPosition;
    bool readyForNext = false;
    uint8_t pos = 0;
    for (uint8_t x = 0; x < BLOCK_WIDTH; x++) {
        for (uint8_t y = 0; y < BLOCK_HEIGHT; y++) {
            if (((blocksArray[_type][_rotation] >> pos) & 1) == true) {
                tempPosition.x = _pos->x + x;
                tempPosition.y = _pos->y + y;

                PositionCorrection(tempPosition, &correction);
            }
            pos++;
        }
    }
    _pos->x += correction.x;
    _pos->y += correction.y;

    if (correction.y < 0) {
        readyForNext = true;
    }
    return readyForNext;
}

const void Tetris::PositionCorrection(Position_t _block, Position_t* _correction) {

    if (_block.x < 0) {
        _correction->x = 1;
    }
    if (_block.x >= SCREEN_WIDTH) {
        _correction->x = -1;
    }
    if (_block.y < 0) {
        _correction->y = 1;
    }
    if (_block.y >= SCREEN_HEIGHT) {
        _correction->y = -1;
    }
}

void Tetris::setup(void)
{
    for (uint8_t i = 0; i < (SCREEN_WIDTH * SCREEN_HEIGHT); ++i) {
        grid[i] = { OUT_OF_SCREEN, OUT_OF_SCREEN };
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
    DrawObject(grid, SCREEN_HEIGHT * SCREEN_WIDTH);
}

void Tetris::HandleAction(Direction_t _dir)
{
    //todo
}

void Tetris::DrawBlock(uint8_t _type, uint8_t _rotation, Position_t _pos)
{
    uint8_t pos = 0;
    for (uint8_t x = 0; x < BLOCK_WIDTH; x++) {
        for (uint8_t y = 0; y < BLOCK_HEIGHT; y++) {
            if (((blocksArray[_type][_rotation] >> pos) & 1) == true) {
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

const Position_t Tetris::GetNewBlockPosition(Position_t* _pos, Direction_t _dir)
{
    if (Direction_t::UP == _dir) {
        _pos->y--;
    }
    else if (Direction_t::DOWN == _dir) {
        _pos->y++;
    }
    else if (Direction_t::LEFT == _dir) {
        _pos->x--;
    }
    else if (Direction_t::RIGHT == _dir) {
        _pos->x++;
    }

    return *_pos;
}

void Tetris::MovoBlockToScreen(void)
{
}
