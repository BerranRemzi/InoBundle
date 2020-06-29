#include "Arduino.h"

#if ARDUINO

#define VK_UP		5
#define VK_DOWN		2
#define VK_RIGHT	3
#define VK_LEFT		4
#define VK_A		0
#define VK_B		0
#define VK_C		0
#define VK_D		0

#endif

bool KB_IsPressed(uint8_t _pin);
void KB_ReadAll(void);
void KB_Reset(void);