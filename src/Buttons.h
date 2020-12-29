#include "Arduino.h"
#include "Utility.h"

#if ARDUINO

#define VK_UP		1
#define VK_DOWN		2
#define VK_RIGHT	3
#define VK_LEFT		0
#define VK_NOT_PRESSED 5
#define VK_A		12
#define VK_B		13
#define VK_X		10
#define VK_Y		11
#define VK_NEXT     14

#define DEBOUNCE_TICK 3

#else

#define VK_A		'S'
#define VK_B		'B'
#define VK_X		'A'
#define VK_Y		'W'
#define VK_NEXT     14
#define VK_NOT_PRESSED 5


#define DEBOUNCE_TICK 1

#endif

#define MOUSE_LEFT  0
#define MOUSE_RIGHT 1

#define KEY_LEFT_SHIFT 0
#define KEY_RIGHT_SHIFT 1

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

void PowerManager_SetSleepTimeout(uint16_t _timeout);
void PowerManager_Refresh(void);
void PowerManager_GotoSleep(void);
void PowerManager_Task(void);

//private
uint8_t AnalogButton_Compute(uint16_t _new_sample);