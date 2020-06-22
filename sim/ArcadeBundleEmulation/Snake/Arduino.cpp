#include "Arduino.h"

void setup() {
    Serial.begin(9600);
}

void loop() {
    //printf("Hello");
    Serial.println("Hello");
    delay(100);
}