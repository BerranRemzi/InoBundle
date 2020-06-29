#include "Keyboard.h"

const uint8_t keyMap[] = {
    VK_UP,
    VK_DOWN,
    VK_RIGHT,
    VK_LEFT,
    VK_A,
    VK_B,
    VK_C,
    VK_D
};

const uint8_t keyMapSize = sizeof(keyMap);

static uint16_t keyData  = 0x0000;

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