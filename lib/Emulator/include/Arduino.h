#ifndef ARDUINO_H
#define ARDUINO_H

//#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <graphics.h>

#define LOW	0
#define HIGH 1


class SoftwareSerial {
public:
	SoftwareSerial();
    void begin(uint32_t _baud);
    void print(char const* const _input);
	void print(int _input);
    void println(char const* const _input);
	void println(int _input);
	bool isOpen();
private:
    bool isSerialOpen = false;
};

extern SoftwareSerial Serial;

void setup();
void loop();
void incrementMillis(uint32_t input);
uint32_t millis();
int random(int _max);
void randomSeed(int16_t);
void wdt_reset();
int Test(int a, int b);


#endif /* ARDUINO_H */