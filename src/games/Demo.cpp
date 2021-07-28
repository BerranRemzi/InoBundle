#include "Demo.h"
#include "Arduino.h"
#include "../InoBundle.h"
uint32_t lastMillis = 0;
uint8_t state = 0;
void Task_Demo(void)
{
    if((unsigned long)(millis() - lastMillis) > 100 ) {
        lastMillis = millis();
        switch(state){
            case 0: break;
            case 1: WriteCol(0, 0xFF); break;
            case 2: WriteCol(7, 0xFF); break;
            case 3: WriteRow(0, 0xFF); break;
            case 4: WriteRow(7, 0xFF); break;
            case 5: 
            Pixel(3, 3, HIGH);
            Pixel(3, 4, HIGH);
            Pixel(4, 3, HIGH);
            Pixel(4, 4, HIGH);
            break;
            case 6: Clear(); break;
            case 7: break;
            default: state = 0; break;
        }            
        state++;
    }
#if 0
    if (GetButton(LEFT_BTN))
    {
        Serial.println("LEFT_BTN");
    }
    if (GetButton(RIGHT_BTN))
    {
        Serial.println("RIGHT_BTN");
    }
    if (GetButton(UP_BTN))
    {
        Serial.println("UP_BTN");
    }
    if (GetButton(DOWN_BTN))
    {
        Serial.println("DOWN_BTN");
    }

    if (GetButton(A_BTN))
    {
        Serial.println("A_BTN");
    }
    if (GetButton(B_BTN))
    {
        Serial.println("B_BTN");
    }
    if (GetButton(X_BTN))
    {
        Serial.println("X_BTN");
    }
    if (GetButton(Y_BTN))
    {
        Serial.println("Y_BTN");
    }
#endif
#if 0
    if (GetButtonDown(LEFT_BTN))
    {
        Serial.println("LEFT_BTN_DOWN");
    }
    if (GetButtonDown(RIGHT_BTN))
    {
        Serial.println("RIGHT_BTN_DOWN");
    }
    if (GetButtonDown(UP_BTN))
    {
        Serial.println("UP_BTN_DOWN");
    }
    if (GetButtonDown(DOWN_BTN))
    {
        Serial.println("DOWN_BTN_DOWN");
    }

    if (GetButtonDown(A_BTN))
    {
        Serial.println("A_BTN_DOWN");
    }
    if (GetButtonDown(B_BTN))
    {
        Serial.println("B_BTN_DOWN");
    }
    if (GetButtonDown(X_BTN))
    {
        Serial.println("X_BTN_DOWN");
    }
    if (GetButtonDown(Y_BTN))
    {
        Serial.println("Y_BTN_DOWN");
    }
    #endif
    delay(100);
}