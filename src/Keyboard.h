#include "Arduino.h"
#include "Utility.h"

#if ARDUINO

#define VK_UP		5
#define VK_DOWN		2
#define VK_RIGHT	3
#define VK_LEFT		4
#define VK_A		A0
#define VK_B		A1
#define VK_X		A2
#define VK_Y		A3

#define DEBOUNCE_TICK 2

#else

#define VK_A		'S'
#define VK_B		'B'
#define VK_X		'A'
#define VK_Y		'W'

#define DEBOUNCE_TICK 1

#endif

void KB_Setup(void);

bool KB_IsKeyUp(uint8_t _pin);
bool KB_IsKeyDown(uint8_t _pin);
bool KB_IsKeyPressed(uint8_t _pin);
bool KB_IsKeyToggled(uint8_t _pin);
bool KB_IsKeyDownLong(uint8_t _pin, uint16_t _tick);
bool KB_IsSinglePressed(uint8_t _pin);

void KB_ReadAll(void);
void KB_Reset(void);
Direction_t KB_GetLastDirection(void);