#include <Arduino.h>

#include <SoftwareSerial.h>

SoftwareSerial logSerial(10, 11); // RX, TX

void setup() {
    // Open serial communications and wait for port to open:
    Serial.begin(9600);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
    }


    Serial.println("Goodnight moon!");

    // set the data rate for the SoftwareSerial port
    logSerial.begin(9600);
    logSerial.println("Hello, world?");
}

void loop() { // run over and over
    if (logSerial.available()) {
        Serial.write(logSerial.read());
    }
    if (Serial.available()) {
        logSerial.write(Serial.read());
    }
}
