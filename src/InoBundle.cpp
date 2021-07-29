#include "Arduino.h"
#include "InoBundle.h"
#include "InoBundle_Cfg.h"
#include <TimerOne.h>

volatile uint16_t screen[SCREEN_HEIGHT];

void Init(void)
{
    for (int y = 0; y < SCREEN_HEIGHT; y++)
    {
        pinMode(row[y], OUTPUT);
    }
    for (int x = 0; x < SCREEN_WIDTH; x++)
    {
        pinMode(col[x], OUTPUT);
    }

    Clear();
    UpdateScreen();

    Timer1.initialize(UPDATE_PERIOD_US);
    Timer1.attachInterrupt(UpdateScreen);
}

unsigned long previousTaskMillis = 0;
void Task(void)
{
    if ((unsigned long)(millis() - previousTaskMillis) > TASK_INTERVAL_MS)
    {
        previousTaskMillis = millis();
        ReadButtons();
    }
}

void UpdateScreen(void)
{
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

boolean GetPixel(int x, int y) {
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

uint16_t decodedButton[2][3];
boolean GetButtonDown(Button_t input)
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
boolean GetButton(Button_t input)
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

#if 0
boolean GetButton(Button_t input)
{
    uint8_t value = 0;
    uint8_t btnGroup = 0;
    boolean val0;
    ComputeButtonParameter(input, &value, &btnGroup);
    
    val0 = (decodedButton[btnGroup][0] >> value) & 1U;
    //boolean val1 = decodedButton[btnGroup][1] >> value & 1U;
    //boolean val2 = decodedButton[btnGroup][2] >> value & 1U;

    return val0;
}
#endif

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
#if 0
#include "wiring_private.h"
#include "pins_arduino.h"

extern uint8_t analog_reference;

int analogRead_Custom(uint8_t pin)
{
    uint8_t low, high;

#if defined(analogPinToChannel)
#if defined(__AVR_ATmega32U4__)
    if (pin >= 18)
        pin -= 18; // allow for channel or pin numbers
#endif
    pin = analogPinToChannel(pin);
#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    if (pin >= 54)
        pin -= 54; // allow for channel or pin numbers
#elif defined(__AVR_ATmega32U4__)
    if (pin >= 18)
        pin -= 18; // allow for channel or pin numbers
#elif defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__)
    if (pin >= 24)
        pin -= 24; // allow for channel or pin numbers
#else
    if (pin >= 14)
        pin -= 14; // allow for channel or pin numbers
#endif

#if defined(ADCSRB) && defined(MUX5)
    // the MUX5 bit of ADCSRB selects whether we're reading from channels
    // 0 to 7 (MUX5 low) or 8 to 15 (MUX5 high).
    ADCSRB = (ADCSRB & ~(1 << MUX5)) | (((pin >> 3) & 0x01) << MUX5);
#endif

    // set the analog reference (high two bits of ADMUX) and select the
    // channel (low 4 bits).  this also sets ADLAR (left-adjust result)
    // to 0 (the default).
#if defined(ADMUX)
#if defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
    ADMUX = (analog_reference << 4) | (pin & 0x07);
#else
    ADMUX = (analog_reference << 6) | (pin & 0x07);
#endif
#endif

    // without a delay, we seem to read from the wrong channel
    //delay(1);

#if defined(ADCSRA) && defined(ADCL)
    // start the conversion
    sbi(ADCSRA, ADSC);

    // ADSC is cleared when the conversion finishes
    while (bit_is_set(ADCSRA, ADSC))
        ;

    // we have to read ADCL first; doing so locks both ADCL
    // and ADCH until ADCH is read.  reading ADCL second would
    // cause the results of each conversion to be discarded,
    // as ADCL and ADCH would be locked when it completed.
    low = ADCL;
    high = ADCH;
#else
    // we dont have an ADC, return 0
    low = 0;
    high = 0;
#endif

    // combine the two bytes
    return (high << 8) | low;
}
#endif