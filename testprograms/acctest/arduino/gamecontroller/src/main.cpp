#include <Arduino.h>
#include <SoftwareSerial.h>

SoftwareSerial logSerial(10, 11); // RX, TX


float read2byteFloat();

void setup() {
    logSerial.begin(9600);
    logSerial.println("---- SETUP_START");

    pinMode(2, INPUT_PULLUP);

    Serial.begin(9600);

    logSerial.println("---- SETUP_DONE");
}

void loop() {

    if (Serial.available()) {
        int prefix = Serial.read();
        if (prefix == 0) {
            logSerial.println("start package");
            int cmd = Serial.read();
            if (cmd == 1) {
                logSerial.print(", setAngle x= ");
                logSerial.print(read2byteFloat());
                logSerial.print(", setAngle y= ");
                logSerial.print(read2byteFloat());
                logSerial.println();
            }
        }
    }
}

float read2byteFloat() {
    int pseudoValue = (Serial.read() - 64) * 127 + (Serial.read() - 64);
    return map(pseudoValue, 0, 16000, -9000, 9000) / 100.0;
}

