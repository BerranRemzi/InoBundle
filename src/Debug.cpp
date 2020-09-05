#include "Debug.h"

uint32_t enterTime[BUFFER_SIZE];
uint32_t leaveTime[BUFFER_SIZE];

void Task_Enter(uint8_t _input){
    enterTime[_input] = micros();
}
void Task_Leave(uint8_t _input){
    leaveTime[_input] = micros();
}

uint32_t Task_Duration(uint8_t _input){
    return (leaveTime[_input] - enterTime[_input]);
}

void Task_PrintAll(void){
    for(uint8_t i = 0; i< BUFFER_SIZE; i++){
        Serial.print(Task_Duration(i));
        if(i != BUFFER_SIZE - 1){
            Serial.print(",");
        }else{
            Serial.println();
        }
    }

}