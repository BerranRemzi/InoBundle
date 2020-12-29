#pragma once
#define SLEEP_FOREVER 	-1
#define ADC_OFF			0
#define BOD_OFF			0

class LowPower {
public:
    static void powerDown(uint8_t, uint8_t, uint8_t);
}LowPower;

void LowPower::powerDown(uint8_t a, uint8_t b, uint8_t c) {

}