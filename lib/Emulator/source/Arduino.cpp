#include "Arduino.h"

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <Windows.h>
#include <ctime>

uint32_t millisTime = 0;

SoftwareSerial::SoftwareSerial(){
}

void SoftwareSerial::begin(uint32_t _baud) {
	isSerialOpen = true;
}
void SoftwareSerial::print(char const* const _input) {
	printf("%s", _input);
}
void SoftwareSerial::print(int _input) {
	printf("%d", _input);
}
void SoftwareSerial::println(char const* const _input) {
	print(_input);
	print("\n");
}
void SoftwareSerial::println(int _input) {
	print(_input);
	print("\n");
}
bool SoftwareSerial::isOpen(){
	return (isSerialOpen);
}

SoftwareSerial Serial;

int Test(int a, int b){
	return (a + b);
};

uint32_t millis(){
	return millisTime;
}

int random(int _max) {
    return (rand() % _max);
}
void randomSeed(int16_t) {
    srand((unsigned int)time(nullptr));
}
void wdt_reset(){}