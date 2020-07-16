#include "Keyboard.h"

struct KeyMap_t {
    uint8_t pin;
    bool lastState;
    uint16_t counter;
    bool isToggled;
};


KeyMap_t keyMap[8] = {
    {VK_UP      },
    {VK_DOWN    },
    {VK_RIGHT   },
    {VK_LEFT    },
    {VK_A},
    {VK_B},
    {VK_X},
    {VK_Y}
};

const uint8_t keyMapSize = 8;

static uint16_t keyDownData = 0U;
static uint16_t keyPressData = 0U;

void KB_Setup(void) {
    for (uint8_t k = 0; k < keyMapSize; k++) {
        pinMode(keyMap[k].pin, INPUT_PULLUP);
    }
}

bool KB_IsKeyUp(uint8_t _pin) {
    bool returnValue = false;

    for (uint8_t k = 0; k < keyMapSize; k++) {
        if (keyMap[k].pin == _pin) {
            returnValue = (keyDownData >> k) & 1U;
            break;
        }
    }

    return !returnValue;
}

bool KB_IsKeyDown(uint8_t _pin) {
    bool returnValue = false;

    for (uint8_t k = 0; k < keyMapSize; k++) {
        if (keyMap[k].pin == _pin) {
            returnValue = (keyDownData >> k) & 1U;
            break;
        }
    }

    return returnValue;
}

bool KB_IsKeyPressed(uint8_t _pin) {
    bool returnValue = false;

    for (uint8_t k = 0; k < keyMapSize; k++) {
        if (keyMap[k].pin == _pin) {
            returnValue = (keyPressData >> k) & 1U;
            break;
        }
    }

    return returnValue;
}

bool KB_IsKeyToggled(uint8_t _pin) {
    bool returnValue = false;

    for (uint8_t k = 0; k < keyMapSize; k++) {
        if (keyMap[k].pin == _pin) {
            returnValue = keyMap[k].isToggled;
            keyMap[k].isToggled = false;
            break;
        }
    }

    return returnValue;
}

bool KB_IsKeyDownLong(uint8_t _pin, uint16_t _tick) {
    bool returnValue = false;

    for (uint8_t k = 0; k < keyMapSize; k++) {
        if (keyMap[k].pin == _pin) {
            if (keyMap[k].counter > _tick) {
                returnValue = true;
                keyMap[k].counter = 0;
            }
            break;
        }
    }

    return returnValue;
}


void KB_ReadAll(void) {
    for (uint8_t k = 0; k < keyMapSize; k++) {

        bool currentState = !digitalRead(keyMap[k].pin);

        if (currentState) {
            keyMap[k].counter++;
        }
        else {
            keyMap[k].counter = 0;
        }

        if (keyMap[k].counter >= DEBOUNCE_TICK) {
            keyDownData |= 1U << k;
            keyPressData |= 1U << k;
        }

        if (keyMap[k].counter == 0) {
            keyDownData &= ~(1U << k);
        }

        if (keyMap[k].counter == 0 || keyMap[k].counter == DEBOUNCE_TICK) {
            if (currentState != keyMap[k].lastState) {
                keyMap[k].isToggled = true;
            }
            keyMap[k].lastState = currentState;
        }
    }
}

void KB_Reset(void) {
    keyPressData = 0x0000;
}

Direction_t KB_GetLastDirection(void) {
    static Direction_t lastDirection = Direction_t::RIGHT;

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