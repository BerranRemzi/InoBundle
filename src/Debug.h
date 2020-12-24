#pragma once
#include "Arduino.h"
#define BUFFER_SIZE 5

#define DEBUG

#ifdef DEBUG
#define TASK_ENTER(INPUT) Task_Enter(INPUT)
#define TASK_LEAVE(INPUT) Task_Leave(INPUT)
#define TASK_DURATION(INPUT) Task_Duration(INPUT)
#define TASK_PRINT_ALL() Task_PrintAll()
#define TASK_PUT_TOTAL() Task_PutTotal()
#else
#define TASK_ENTER(INPUT)
#define TASK_LEAVE(INPUT)
#define TASK_DURATION(INPUT)
#define TASK_PRINT_ALL()
#define TASK_PUT_TOTAL()
#endif

void Task_Enter(uint8_t _input);
void Task_Leave(uint8_t _input);
uint32_t Task_Duration(uint8_t _input);
void Task_PrintAll(void);
void Task_PutTotal(void);