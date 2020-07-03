#include "Keyboard.h"

const uint8_t keyMap[] = {
    VK_UP,
    VK_DOWN,
    VK_RIGHT,
    VK_LEFT,
    'W',
    'S',
    'D',
    'A'
};

const uint8_t keyMapSize = sizeof(keyMap);

static uint16_t keyData  = 0x0000;

void KB_Setup(void){
    pinMode(VK_UP, INPUT_PULLUP);
	pinMode(VK_DOWN, INPUT_PULLUP);
	pinMode(VK_LEFT, INPUT_PULLUP);
	pinMode(VK_RIGHT, INPUT_PULLUP);
    
    /* TODO: add buttons A, B, C, and D  */
}

bool KB_IsPressed(uint8_t _pin){
    bool returnValue = false;

    for(uint8_t k = 0; k < keyMapSize;k++){
        if(keyMap[k] == _pin){
            returnValue = (keyData >> k) & 1U;
            break;
        }
    }

    return returnValue;
}

void KB_ReadAll(void){
    for(uint8_t k = 0; k < keyMapSize;k++){

        if(!digitalRead(keyMap[k])){
            keyData |= 1U << k;
        }else{
            //keyData &= ~(1U << k);
        }
    }
}

void KB_Reset(void){
    keyData = 0x0000;
}

Direction_t KB_GetLastDirection(void) {
	static Direction_t lastDirection = DIR_RIGHT;

	if (KB_IsPressed(VK_UP) && DIR_UP != lastDirection) {
		lastDirection = DIR_UP;
	}

	if (KB_IsPressed(VK_DOWN) && DIR_DOWN != lastDirection) {
		lastDirection = DIR_DOWN;
	}

	if (KB_IsPressed(VK_RIGHT) && DIR_RIGHT != lastDirection) {
		lastDirection = DIR_RIGHT;
	}

	if (KB_IsPressed(VK_LEFT) && DIR_LEFT != lastDirection) {
		lastDirection = DIR_LEFT;
	}
	KB_Reset();
	return lastDirection;
}