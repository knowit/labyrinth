#include <Arduino.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

SoftwareSerial logSerial(10, 11); // RX, TX

String state = "unknow";
float setXAngle = 0;
float setYAngle = 0;

float read2byteFloat();

void setSystemStateState(const char *newState);

void updateDisplay() {
    display.clearDisplay();
    display.setTextSize(1);
    display.cp437(true);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println(state);
    display.setCursor(0, 10);
    display.println(setXAngle);
    display.setCursor(0, 20);
    display.println(setYAngle);
    display.display();
}

void setup() {
    logSerial.begin(9600);
    logSerial.println("---- SETUP_START");

    pinMode(2, INPUT_PULLUP);

    logSerial.print("init display ... ");
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
        Serial.println(F("SSD1306 allocation failed"));
        for(;;); // Don't proceed, loop forever
    }
    setSystemStateState("init");
    logSerial.println("OK");

    Serial.begin(9600);

    logSerial.println("---- SETUP_DONE");
    setSystemStateState("Running");
}

void setSystemStateState(const char *newState) {
    state = newState;
    updateDisplay();
}

void loop() {

    if (Serial.available()) {
        int prefix = Serial.read();
        if (prefix == 0) {
            logSerial.println("start package");
            int cmd = Serial.read();
            if (cmd == 1) {
                logSerial.print(", setAngle x= ");
                setXAngle = read2byteFloat();
                logSerial.print(setXAngle);
                logSerial.print(", setAngle y= ");
                setYAngle = read2byteFloat();
                logSerial.print(setYAngle);
                logSerial.println();
            }
            updateDisplay();
        }
    }
}

float read2byteFloat() {
    int pseudoValue = (Serial.read() - 64) * 127 + (Serial.read() - 64);
    return map(pseudoValue, 0, 16000, -9000, 9000) / 100.0;
}

