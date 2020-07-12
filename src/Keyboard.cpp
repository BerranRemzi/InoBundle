#include "Keyboard.h"

struct KeyMap_t{
    uint8_t pin;
    bool lastState;
    int16_t counter;
    bool isToggled;
};


KeyMap_t keyMap[8] = {
    {VK_UP      },
    {VK_DOWN    },
    {VK_RIGHT   },
    {VK_LEFT    },
    {'W'},
    {'S'},
    {'D'},
    {'A'}
};

const uint8_t keyMapSize = 8;

static uint16_t keyData  = 0U;

void KB_Setup(void){
    pinMode(VK_UP, INPUT_PULLUP);
	pinMode(VK_DOWN, INPUT_PULLUP);
	pinMode(VK_LEFT, INPUT_PULLUP);
	pinMode(VK_RIGHT, INPUT_PULLUP);
    
    /* TODO: add buttons A, B, C, and D  */
}

bool KB_IsKeyUp(uint8_t _pin){
    bool returnValue = false;

    for(uint8_t k = 0; k < keyMapSize;k++){
        if(keyMap[k].pin == _pin){
            returnValue = (keyData >> k) & 1U;
            break;
        }
    }

    return !returnValue;
}

bool KB_IsKeyDown(uint8_t _pin){
    bool returnValue = false;

    for(uint8_t k = 0; k < keyMapSize;k++){
        if(keyMap[k].pin == _pin){
            returnValue = (keyData >> k) & 1U;
            //keyData &= ~(1U << k);
            break;
        }
    }

    return returnValue;
}

bool KB_IsKeyToggled(uint8_t _pin){
    bool returnValue = false;
    
    for(uint8_t k = 0; k < keyMapSize;k++){
        if(keyMap[k].pin == _pin){
            returnValue = keyMap[k].isToggled;
            keyMap[k].isToggled = false;
            break;
        }
    }

    return returnValue;
}

bool KB_IsKeyDownLong(uint8_t _pin, uint16_t _tick){
    bool returnValue = false;

    for(uint8_t k = 0; k < keyMapSize;k++){
        if(keyMap[k].pin == _pin){
            if(keyMap[k].counter > _tick){
                returnValue = true;
                keyMap[k].counter = 0;
            }
            break;
        }
    }

    return returnValue;
}
#define DEBOUNCE_TICK 2

void KB_ReadAll(void){
    for(uint8_t k = 0; k < keyMapSize;k++){

        bool currentState = !digitalRead(keyMap[k].pin);

        if(currentState){
            if (keyMap[k].counter < DEBOUNCE_TICK)
            keyMap[k].counter++;
        }else{
            if(keyMap[k].counter > 0){
               keyMap[k].counter--;
            }
        }

        if(keyMap[k].counter == 0){
            keyData &= ~(1U << k);
        }
        if(keyMap[k].counter == DEBOUNCE_TICK){
            keyData |= 1U << k;
        }

        if(keyMap[k].counter == 0 || keyMap[k].counter == DEBOUNCE_TICK){
            if(currentState != keyMap[k].lastState){
                keyMap[k].isToggled = true;
            }
            keyMap[k].lastState = currentState;
        }
    }
}

void KB_Reset(void){
    keyData = 0x0000;
}

Direction_t KB_GetLastDirection(void) {
	static Direction_t lastDirection = Direction_t::RIGHT;

	if (KB_IsKeyDown(VK_UP) && Direction_t::UP != lastDirection) {
		lastDirection = Direction_t::UP;
	}

	if (KB_IsKeyDown(VK_DOWN) && Direction_t::DOWN != lastDirection) {
		lastDirection = Direction_t::DOWN;
	}

	if (KB_IsKeyDown(VK_RIGHT) && Direction_t::RIGHT != lastDirection) {
		lastDirection = Direction_t::RIGHT;
	}

	if (KB_IsKeyDown(VK_LEFT) && Direction_t::LEFT != lastDirection) {
		lastDirection = Direction_t::LEFT;
	}
	KB_Reset();
	return lastDirection;
}