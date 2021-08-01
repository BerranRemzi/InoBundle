#include "Arduino.h"
#include "InoBundle.h"
#include "InoBundle_Cfg.h"
#include <stdint.h>
#ifdef ARDUINO
#include <TimerOne.h>
#include <avr/wdt.h>
#endif

volatile uint16_t screen[SCREEN_HEIGHT];

void Init(void)
{
    #ifdef ARDUINO
    wdt_enable(WDTO_250MS);

    for (int y = 0; y < SCREEN_HEIGHT; y++)
    {
        pinMode(row[y], OUTPUT);
    }
    for (int x = 0; x < SCREEN_WIDTH; x++)
    {
        pinMode(col[x], OUTPUT);
    }
    
    Timer1.initialize(UPDATE_PERIOD_US);
    Timer1.attachInterrupt(UpdateScreen);

    #endif

    Clear();
    UpdateScreen();
}

void InoBundle(void)
{
    ReadButtons();
    wdt_reset();
}

void UpdateScreen(void)
{
    #ifdef ARDUINO
    static uint8_t y = 0;
    digitalWrite(row[y], ROW_OFF);

    y++;
    if (y >= SCREEN_HEIGHT)
    {
        y = 0;
    }

    for (uint8_t x = 0; x < SCREEN_WIDTH; x++)
    {
        if (COLUMN_ON)
        {
            digitalWrite(col[x], !(screen[y] >> x) & 1U);
        }
        else
        {
            digitalWrite(col[x], (screen[y] >> x) & 1U);
        }
    }
    digitalWrite(row[y], ROW_ON);
    #endif
}

void Pixel(int x, int y, bool state)
{
    if (state)
    {
        screen[y] |= 1UL << x;
    }
    else
    {
        screen[y] &= ~(1UL << x);
    }
}

bool GetPixel(int x, int y) {
    return ((screen[y] >> x) & 1U);
}

void WriteRow(int y, uint16_t value)
{
    screen[y] = value;
}

void WriteCol(int x, uint16_t value)
{
    for (int y = 0; y < SCREEN_HEIGHT; y++)
    {
        Pixel(x, y, (value >> y) & 1U);
    }
}

void Clear(void)
{
    for (int y = 0; y < SCREEN_HEIGHT; y++)
    {
        screen[y] = 0x00;
    }
}

volatile uint16_t * GetScreenBuffer(int *width, int *height){
    *width = SCREEN_WIDTH;
    *height = SCREEN_HEIGHT;

    return (screen);
}

uint16_t decodedButton[2][3];

#ifdef ARDUINO
bool GetButtonDown(Button_t input)
{
    uint8_t value = input;
    uint8_t btnGroup = 0;
    boolean returnValue = false;
    boolean val0;
    boolean val1;
    boolean val2;
    if (value > LEFT_BTN_GROUP)
    {
        value -= LEFT_BTN_GROUP + 1;
        btnGroup = 0;
    }
    else
    {
        value--;
        btnGroup = 1;
    }
    val0 = decodedButton[btnGroup][0] >> value & 1U;
    val1 = decodedButton[btnGroup][1] >> value & 1U;
    val2 = decodedButton[btnGroup][2] >> value & 1U;   
    returnValue = val0 && val1 && !val2;

    if(true == returnValue){
        decodedButton[btnGroup][2] |= 1U << value;
    }

    return returnValue;
}
bool GetButton(Button_t input)
{
    uint8_t value = input;
    uint8_t btnGroup = 0;
    boolean val0;
    boolean val1;
    boolean val2;
    if (value > LEFT_BTN_GROUP)
    {
        value -= LEFT_BTN_GROUP + 1;
        btnGroup = 0;
    }
    else
    {
        value--;
        btnGroup = 1;
    }
    val0 = decodedButton[btnGroup][0] >> value & 1U;
    val1 = decodedButton[btnGroup][1] >> value & 1U;
    val2 = decodedButton[btnGroup][2] >> value & 1U;   

    return val0 && val1;
}

void ComputeButtonParameter(Button_t input, uint8_t * value, uint8_t * btnGroup){
    if ((*value) > (uint8_t)LEFT_BTN_GROUP)
    {
        (*value) -= (uint8_t)LEFT_BTN_GROUP + 1u;
        (*btnGroup) = 0;
    }
    else
    {
        (*value)--;
        (*btnGroup) = 1;
    }
}
extern const int button[][2];

uint8_t AnalogButton_Compute(uint16_t _newSample)
{
    int16_t lastDiff = 0;
    uint8_t returnValue = 0;
    for (uint8_t p = 0; p < 9; p++)
    {
        int16_t newDiff = (int16_t)abs((int16_t)_newSample - (int16_t)button[p][1]);
        if ((lastDiff >= newDiff) || (p == 0))
        {
            lastDiff = newDiff;
            returnValue = button[p][0];
        }
    }
    return returnValue;
}

void ReadButtons(void)
{
    static uint8_t buttonGroup = 0;
    decodedButton[buttonGroup][2] = decodedButton[buttonGroup][1];
    decodedButton[buttonGroup][1] = decodedButton[buttonGroup][0];
    decodedButton[buttonGroup][0] = AnalogButton_Compute(analogRead(adc[buttonGroup]));
    
    if (0 == buttonGroup)
    {
        buttonGroup = 1;
    }
    else
    {
        buttonGroup = 0;
    }
}
#else

bool GetButtonDown(Button_t input){return false;}
extern int key;
bool GetButton(Button_t input){

    bool returnValue = false;
    if(input == UP_BTN && key == KEY_UP){
        returnValue = true;
    }
    if(input == DOWN_BTN && key == KEY_DOWN){
        returnValue = true;
    }
    if(input == RIGHT_BTN && key == KEY_RIGHT){
        returnValue = true;
    }
    if(input == LEFT_BTN && key == KEY_LEFT){
        returnValue = true;
    }
    return (returnValue);
}
void ReadButtons(void){}
void ComputeButtonParameter(Button_t input, uint8_t * value, uint8_t * btnGroup){}

#endif /* ARDUINO */