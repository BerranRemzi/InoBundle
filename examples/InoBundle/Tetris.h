#pragma once

#include "Game.h"
#include "Utility.h"
#include "Arduino.h"
#include "Keyboard.h"
#include "ArcadeBundle.h"

#define BLOCK_HEIGHT 4
#define BLOCK_WIDTH 4
#define BRICK_ARRAY_SIZE (BLOCK_HEIGHT*BLOCK_WIDTH)

class Tetris :public Game {
public:
    Tetris(void);
    void update() override;
private:
    void setup(void);
    void reset(void);
    void render(void);
    void HandleAction(Direction_t _dir);
    void DrawBlock(uint8_t _type, uint8_t _rotation, Position_t _pos);
    Position_t ReadDirection(Position_t* _pos);

    bool CollisionStoredBlock(void);
    bool ColiisionBoardLimit(void);

    Position_t* storedBricks = matrix;
    Position_t block[BRICK_ARRAY_SIZE];
    GameState state = GameState::GAME_RUN;
    Timer* timer;
    Timer* spawner;
    Position_t position = { 0,0 };
    uint8_t rotation = 0;
    uint8_t type = 0;

    const uint16_t block_i[4] = { 0x0F00, 0x2222, 0x00F0, 0x4444 };
    const uint16_t block_j[4] = { 0x44C0, 0x8E00, 0x6440, 0x0E20 };
    const uint16_t block_l[4] = { 0x4460, 0x0E80, 0xC440, 0x2E00 };
    const uint16_t block_o[4] = { 0xCC00, 0xCC00, 0xCC00, 0xCC00 };
    const uint16_t block_s[4] = { 0x06C0, 0x8C40, 0x6C00, 0x4620 };
    const uint16_t block_t[4] = { 0x0E40, 0x4C40, 0x4E00, 0x4640 };
    const uint16_t block_z[4] = { 0x0C60, 0x4C80, 0xC600, 0x2640 };

    const uint16_t* blocksArray[7] = { block_i, block_j, block_l, block_o, block_s, block_t, block_z };
};