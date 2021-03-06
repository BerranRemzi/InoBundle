#include "Buttons.h"
#include "LowPower.h"

typedef struct AnalogButtonConfig_t
{
    const uint16_t* p_analogButton;
    uint8_t count;
} AnalogButtonConfig_t;

enum Button_t
{
    BUTTON_1,
    BUTTON_2,
    BUTTON_3,
    BUTTON_4,
    BUTTON_NEXT,
    BUTTON_NOT_PRESSED,
    BUTTON_MAX_NUMBER /**< Number of buttons in the system */
};

const uint16_t buttonValues[] = { 133, 177, 196, 212, 231, 255 };
AnalogButtonConfig_t buttonConfig = { buttonValues, BUTTON_MAX_NUMBER };
AnalogButtonConfig_t* p_buttonConfig = &buttonConfig;

uint8_t adc1;
uint8_t adc2;

struct KeyMap_t
{
    uint8_t pin;
    bool lastState;
    uint16_t counter;
    bool isToggled;
};

KeyMap_t keyMap[9] = {
    {VK_UP},
    {VK_DOWN},
    {VK_RIGHT},
    {VK_LEFT},
    {VK_A},
    {VK_B},
    {VK_X},
    {VK_Y},
    {VK_NEXT} };

const uint8_t keyMapSize = 9;

static uint16_t keyDownData = 0U;
static uint16_t keyPressData = 0U;
#if 0
uint8_t AnalogButton_Compute(uint16_t _new_sample)
{
    uint8_t _temp_command = p_buttonConfig->count - 1;
    uint16_t abs_difference = abs(_new_sample - p_buttonConfig->p_analogButton[0]);

    for (uint8_t position = 0; position < p_buttonConfig->count; position++)
    {
        uint16_t temp_difference = abs(_new_sample - p_buttonConfig->p_analogButton[position]);
        if (abs_difference >= temp_difference)
        {
            abs_difference = temp_difference;
            _temp_command = position;
        }
    }

    return _temp_command;
}
#else
uint8_t AnalogButton_Compute(uint16_t _newSample) {
    int16_t lastDiff = 0;
    uint8_t returnValue = 0;
    for (uint8_t p = 0; p < p_buttonConfig->count; p++) {
        int16_t newDiff = (int16_t)abs((int16_t)_newSample - (int16_t)p_buttonConfig->p_analogButton[p]);
        if ((lastDiff >= newDiff) || (p == 0)) {
            lastDiff = newDiff;
            returnValue = p;
        }
    }
    return returnValue;
}
#endif

bool GetAnalogButton(uint8_t _pin)
{
    bool returnValue = false;

    /* Arrow buttons */
    uint8_t btn1 = AnalogButton_Compute(adc1);
    /* Control buttons */
    uint8_t btn2 = AnalogButton_Compute(adc2);
    if ((VK_NOT_PRESSED != btn1) || (VK_NOT_PRESSED != btn2)) {
        PowerManager_Refresh();
    }
    btn2 += 10;

    if ((btn1 == _pin) || (btn2 == _pin))
    {
        returnValue = true;
    }
    return returnValue;
}

void KB_Setup(void)
{
    /*for (uint8_t k = 0; k < keyMapSize; k++) {
        pinMode(keyMap[k].pin, INPUT_PULLUP);
    }*/
}

bool KB_IsKeyUp(uint8_t _pin)
{
    bool returnValue = false;

    for (uint8_t k = 0; k < keyMapSize; k++)
    {
        if (keyMap[k].pin == _pin)
        {
            returnValue = (keyDownData >> k) & 1U;
            break;
        }
    }

    return !returnValue;
}

bool KB_IsKeyDown(uint8_t _pin)
{
    bool returnValue = false;

    for (uint8_t k = 0; k < keyMapSize; k++)
    {
        if (keyMap[k].pin == _pin)
        {
            returnValue = (keyDownData >> k) & 1U;
            break;
        }
    }

    return returnValue;
}

bool KB_IsKeyPressed(uint8_t _pin)
{
    bool returnValue = false;

    for (uint8_t k = 0; k < keyMapSize; k++)
    {
        if (keyMap[k].pin == _pin)
        {
            returnValue = (keyPressData >> k) & 1U;
            break;
        }
    }

    return returnValue;
}

bool KB_IsKeyToggled(uint8_t _pin)
{
    bool returnValue = false;

    for (uint8_t k = 0; k < keyMapSize; k++)
    {
        if (keyMap[k].pin == _pin)
        {
            returnValue = keyMap[k].isToggled;
            keyMap[k].isToggled = false;
            break;
        }
    }

    return returnValue;
}

bool KB_IsKeyDownLong(uint8_t _pin, uint16_t _tick)
{
    bool returnValue = false;

    for (uint8_t k = 0; k < keyMapSize; k++)
    {
        if (keyMap[k].pin == _pin)
        {
            if (keyMap[k].counter > _tick)
            {
                returnValue = true;
                keyMap[k].counter = 0;
            }
            break;
        }
    }

    return returnValue;
}

bool KB_IsSinglePressed(uint8_t _pin)
{
    return (KB_IsKeyToggled(_pin) && KB_IsKeyDown(_pin));
}

void KB_ReadAll(void)
{
    adc1 = analogRead(A1) >> 2;
    adc2 = analogRead(A0) >> 2;
    for (uint8_t k = 0; k < keyMapSize; k++)
    {
        bool currentState = GetAnalogButton(keyMap[k].pin);

        if (currentState)
        {
            keyMap[k].counter++;
        }
        else
        {
            keyMap[k].counter = 0;
        }

        if (keyMap[k].counter >= DEBOUNCE_TICK)
        {
            keyDownData |= 1U << k;
            keyPressData |= 1U << k;
        }
        else if (keyMap[k].counter == 0)
        {
            keyDownData &= ~(1U << k);
        }

        if (keyMap[k].counter == 0 || keyMap[k].counter == DEBOUNCE_TICK)
        {
            if (currentState != keyMap[k].lastState)
            {
                keyMap[k].isToggled = true;
            }
            keyMap[k].lastState = currentState;
        }
    }
}

void KB_Reset(void)
{
    keyPressData = 0;
}

Direction_t KB_GetLastDirection(void)
{
    static Direction_t lastDirection = Direction_t::RIGHT;

    if (KB_IsKeyPressed(VK_UP) && Direction_t::UP != lastDirection)
    {
        lastDirection = Direction_t::UP;
    }

    if (KB_IsKeyPressed(VK_DOWN) && Direction_t::DOWN != lastDirection)
    {
        lastDirection = Direction_t::DOWN;
    }

    if (KB_IsKeyPressed(VK_RIGHT) && Direction_t::RIGHT != lastDirection)
    {
        lastDirection = Direction_t::RIGHT;
    }

    if (KB_IsKeyPressed(VK_LEFT) && Direction_t::LEFT != lastDirection)
    {
        lastDirection = Direction_t::LEFT;
    }
    KB_Reset();
    return lastDirection;
}


uint16_t timeoutCounter = 0;
uint16_t sleepTimeout = 30;

void PowerManager_SetSleepTimeout(uint16_t _timeout) {
    sleepTimeout = _timeout;
}
void PowerManager_Refresh(void) {
    timeoutCounter = 0;
}

void PowerManager_GotoSleep(void) {
    for (int i = 0; i < 16; i++) {
        pinMode(i, INPUT);
    }
    pinMode(A0, INPUT);
    pinMode(A1, INPUT);
    pinMode(A2, INPUT);
    pinMode(A3, INPUT);
    pinMode(A4, INPUT);
    pinMode(A5, INPUT);

    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
    delay(100);
}

void PowerManager_Task(void) {
    if (timeoutCounter > sleepTimeout) {
        
        PowerManager_GotoSleep();
        /* Sleep forever and wait RESET signal */
    }
    timeoutCounter++;
}