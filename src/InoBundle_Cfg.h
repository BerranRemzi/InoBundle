#ifndef INO_BUNDLE_CFG_H
#define INO_BUNDLE_CFG_H

#define UPDATE_PERIOD_US 1000
#define SCREEN_HEIGHT 8
#define SCREEN_WIDTH 8

#define LED_1088

const uint8_t row[] = {4, 3, 8, 12, A4, 9, A3, 5};
const uint8_t col[] = {2, A2, 13, 6, 10, 11, A5, 7};
const uint8_t adc[] = {A0, A1};

const int button[][2] = {
    {0x00, 1023},
    {0x01, 840},
    {0x04, 771},
    {0x08, 692},
    {0x05, 662},
    {0x09, 603},
    {0x02, 510},
    {0x06, 439},
    {0x0A, 411}};

typedef enum
{
    RIGHT_BTN_GROUP,
    UP_BTN,
    DOWN_BTN,
    LEFT_BTN,
    RIGHT_BTN,
    LEFT_BTN_GROUP,
    A_BTN,
    B_BTN,
    X_BTN,
    Y_BTN
}Button_t;

#endif /* INO_BUNDLE_CFG_H */