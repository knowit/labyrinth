#include <Arduino.h>

void setup() {
    Serial.begin(115200);
    pinMode(2, INPUT);
    pinMode(13, OUTPUT);
}

void loop() {
    delay(20);
    digitalWrite(13, analogRead(2) > 990 ? 1 : 0);
}