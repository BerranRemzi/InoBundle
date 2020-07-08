#pragma once

#include "Arduino.h"
#include <SPI.h>
#include "ArcadeBundle_Types.h"

#define SPI_CS_PIN 10

//the opcodes for the MAX7221 and MAX7219
enum OP_CODE{
    OP_NOOP, 
    OP_DIGIT0, 
    OP_DIGIT1, 
    OP_DIGIT2, 
    OP_DIGIT3, 
    OP_DIGIT4,
    OP_DIGIT5,
    OP_DIGIT6, 
    OP_DIGIT7, 
    OP_DECODEMODE, 
    OP_INTENSITY,   
    OP_SCANLIMIT,  
    OP_SHUTDOWN,    
    OP_DISPLAYTEST = 15
};

void AB_HAL_InitPins(void);
void AB_HAL_Init(void);
void AB_HAL_SpiTransfer(volatile uint8_t opcode, volatile uint8_t data);
void AB_HAL_ClearDisplay(void);
void AB_HAL_Shutdown(bool b);
void AB_HAL_UpdateScreen(void);
void AB_HAL_DecodeMode(void);
void AB_HAL_DisplayTest(void);
void AB_HAL_SetScanLimit(uint8_t _limit);
void AB_HAL_SetIntensity( uint8_t intensity);
