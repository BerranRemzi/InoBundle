#include "Arduino.h"
#include "Utility.h"

#if ARDUINO

#define VK_UP		5
#define VK_DOWN		2
#define VK_RIGHT	3
#define VK_LEFT		4
#define VK_A		-1
#define VK_B		-1
#define VK_C		-1
#define VK_D		-1

#endif
void KB_Setup(void);
bool KB_IsPressed(uint8_t _pin);
void KB_ReadAll(void);
void KB_Reset(void);
Direction_t KB_GetLastDirection(void);