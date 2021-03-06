#pragma once

#include "Arduino.h"
#include "ArcadeBundle_Types.h"

#define COLUMN_OFF LOW
#define COLUMN_ON (!COLUMN_OFF)

#define ROW_OFF LOW
#define ROW_ON (!ROW_OFF)

void AB_HAL_Init(void);
void AB_HAL_UpdateScreen(void);
void AB_HAL_AnalogPrescaler(uint8_t _prescaler);
void WriteRows(uint8_t _state);
void WriteCols(uint8_t _state);
