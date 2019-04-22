#include <Arduino.h>
// #include <SoftwareSerial.h>
#include <SPI.h>
#include <TaskScheduler.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <utility/imumaths.h>
#include "Axis.h"
#include "BNOReader.h"


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
// SoftwareSerial logSerial(10, 11); // RX, TX


String state = "";
String task = "";
bool showTaskParam = false;
float taskParam = 0;

float setYAngle = 0;
float bnoY = 0;

float read2byteFloat();

Axis xAxis(6);
BNOReader bnoReader;

void setSystemStateState(const char *newState);

void readAndParseCommands();

void addAndEnableTask(Task &task);


void xAxisReportState() {
    xAxis.reportState();
}

Task xAxisReportStateTask(200, TASK_FOREVER, &xAxisReportState);


void updateDisplay() {
    int xPosBno = 38;
    int xPosSpeed = 80;
    int xPosSet = 5;
    display.clearDisplay();
    display.setTextSize(1);
    display.cp437(true);
    display.setTextColor(WHITE);


    display.setCursor(0, 0);
    display.println(state);

    display.setCursor(0, 8);
    display.println(task);
    if (showTaskParam) {
        display.setCursor(75, 8);
        display.println(taskParam);
    }


    display.setCursor(xPosSet, 8 * 2);
    display.println(xAxis.setpointAngle);
    display.setCursor(xPosBno, 8 * 2);
    display.println(xAxis.bnoAngle);
    display.setCursor(xPosSpeed, 8 * 2);
    display.println(xAxis.xSpeedAdjusted);

    display.setCursor(xPosSet, 8 * 3);
    display.println(setYAngle);
    display.setCursor(xPosBno, 8 * 3);
    display.println(bnoY);

    display.display();
}

Task updateDisplayTask(200, TASK_FOREVER, &updateDisplay);

Scheduler runner;

void setupTasks() {
    runner.init();
    addAndEnableTask(updateDisplayTask);
    addAndEnableTask(xAxisReportStateTask);
}


void setSystemStateState(const char *newState) {
    state = newState;
}

void setTask(const char *newTask) {
    task = newTask;
}

void setup() {

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
        // SSD1306 allocation failed
        for (;;); // Don't proceed, loop forever
    }
    setSystemStateState("init");
    updateDisplay();

    setTask("init serial");
    updateDisplay();
    Serial.begin(115200);

    setTask("init bno");
    updateDisplay();

    setTask("init BNO ..DONE");
    updateDisplay();

    xAxis.setup();
    bnoReader.setup();

    setupTasks();

    setSystemStateState("Running");
    setTask("");
}


void addAndEnableTask(Task &task) {
    runner.addTask(task);
    task.enable();
}

void loop() {

    runner.execute();

    readAndParseCommands();

    /*
    sensors_event_t event;
    bno.getEvent(&event);

    if (xAxis.bnoAngle != event.orientation.y || bnoY != event.orientation.z) {
        xAxis.bnoAngle = event.orientation.y;
        bnoY = event.orientation.z;
    }
     */

    bnoReader.update();
    xAxis.bnoAngle = bnoReader.xAngle;
    bnoY = bnoReader.yAngle;
    xAxis.update();


}

void readAndParseCommands() {
    if (Serial.available() >= 3) {
        int cmd = Serial.read();
        if (cmd < 64) {
            if (cmd == 1) { // setAngle
                xAxis.setpointAngle = (map(read2byteFloat(), 0, 16000, 0, 100 * 100) / 100.0) - 50;
                float y = read2byteFloat();
                if (y != -999) {
                    setYAngle = map(y, 0, 16000, -90, 90);
                }
            }
            if (cmd == 2) { // set X Kp
                float t = read2byteFloat();
                xAxis.setKp(map(t, 0, 16000, 0, 100 * 100) / 100.0);
                setTask("x Kp: ");
                taskParam = xAxis.GetKp();
                showTaskParam = true;
            }
            if (cmd == 3) { // set X Ki
                float t = read2byteFloat();
                xAxis.setKi(map(t, 0, 16000, 0, 100 * 100) / 100.0);
                setTask("x Ki: ");
                taskParam = xAxis.GetKi();
                showTaskParam = true;
            }
            if (cmd == 4) { // set X Kd
                float t = read2byteFloat();
                if (t >= 0) {
                    xAxis.setKd(map(t, 0, 16000, 0, 100 * 100) / 100.0);
                    setTask("x Kd: ");
                    taskParam = xAxis.GetKd();
                    showTaskParam = true;
                }
            }
            if (cmd == 6) { // set X minimum speed
                float f = read2byteFloat();
                if (f >= 0) {
                    xAxis.xMinSpeed = map(f, 0, 16000, 0, 100 * 100) / 100.0;
                    setTask("x min spd: ");
                    taskParam = xAxis.xMinSpeed;
                    showTaskParam = true;
                }
            }
            if (cmd == 7) { // set X manual
                float f = read2byteFloat();
                if (f >= 0) {
                    xAxis.xManualSpeed = map(f, 0, 16000, 0, 180 * 100) / 100.0;
                    setTask("x man spd: ");
                    taskParam = xAxis.xManualSpeed;
                    showTaskParam = true;
                }
            }
        }
    }
}

float read2byteFloat() {
    int msb = Serial.read();
    if (msb < 64) return -999;
    int lsb = Serial.read();
    if (lsb < 64) return -999;
    return (msb - 64) * 192 + (lsb - 64);
}

