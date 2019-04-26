#include <Arduino.h>
// #include <SoftwareSerial.h>
#include <SPI.h>
#include <TaskScheduler.h>
#include <Wire.h>
#include <Wire.h>
#include <utility/imumaths.h>
#include "Axis.h"
#include "Util.h"
#include "BNOReader.h"
#include "SSD1306Display.h"

Axis xAxis(7, 1, 1, 2);
Axis yAxis(6, 0, 3, 4);
BNOReader bnoReader;
SSD1306Display display(xAxis, yAxis);

void readAndParseCommands();

void addAndEnableTask(Task &task);

double handle2ByteFloatParameter(const char *parameterName, int minValue, int maxValue);

void reportAxisStates() {
    xAxis.reportState();
    yAxis.reportState();
}

Task reportAxisStatesTask(200, TASK_FOREVER, &reportAxisStates);

void updateDisplay() {
    display.updateDisplay();
}

Task updateDisplayTask(200, TASK_FOREVER, &updateDisplay);

Scheduler runner;

void setupTasks() {
    runner.init();
    addAndEnableTask(updateDisplayTask);
    addAndEnableTask(reportAxisStatesTask);
}


void setup() {

    Serial1.begin(115200);
    Serial1.println("____SETUP_BEGIN____");

    int displaySetupResult = display.setup();
    if (displaySetupResult != 0) {
        for (;;);
    }

    display.state = "init";
    updateDisplay();

    display.task = "init serial";
    updateDisplay();
    Serial.begin(115200);

    display.task = "init BNO";
    updateDisplay();

    display.task = "init BNO";
    int bnoSetupResult = bnoReader.setup();
    if (bnoSetupResult != 0) {
        display.task = "init BNO ERROR!";
        updateDisplay();
        for (;;);
    }
    display.task = "init BNO - OK";
    updateDisplay();

    xAxis.setup();
    yAxis.setup();

    setupTasks();

    display.state = "Running";
    display.task = "";

    Serial1.println("____SETUP_END______");
}


void addAndEnableTask(Task &task) {
    runner.addTask(task);
    task.enable();
}

void loop() {
    runner.execute();

    readAndParseCommands();

    bnoReader.update();

    xAxis.bnoAngle = bnoReader.xAngle;
    yAxis.bnoAngle = bnoReader.yAngle;
    xAxis.update();
    yAxis.update();
}

void readAndParseCommands() {
    if (Serial.available() >= 3) {
        int cmd = Serial.read();
        if (cmd == 1) { // set x angle
            xAxis.setpointAngle = (map(read2byteFloat(), 0, 16000, 0, 100 * 100) / 100.0) - 50;
        }
        if (cmd == 11) { // set y angle
            yAxis.setpointAngle = (map(read2byteFloat(), 0, 16000, 0, 100 * 100) / 100.0) - 50;
        }
        if (cmd == 2) {
            xAxis.setKp(handle2ByteFloatParameter("X Kp: ", 0, 100));
        }
        if (cmd == 12) {
            yAxis.setKp(handle2ByteFloatParameter("Y Kp: ", 0, 100));
        }
        if (cmd == 3) {
            xAxis.setKi(handle2ByteFloatParameter("X Ki: ", 0, 100));
        }
        if (cmd == 13) {
            yAxis.setKi(handle2ByteFloatParameter("Y Ki: ", 0, 100));
        }
        if (cmd == 4) {
            xAxis.setKd(handle2ByteFloatParameter("X Kd: ", 0, 100));
        }
        if (cmd == 5) {
            xAxis.xMinSpeed = handle2ByteFloatParameter("x min spd: ", 0, 100);
        }
    }
}

double handle2ByteFloatParameter(const char *parameterName, int minValue, int maxValue) {
    float t = read2byteFloat();
    double value = map(t, 0, 16000, minValue, maxValue * 100) / 100.0;
    display.task = parameterName;
    display.taskParam = value;
    display.showTaskParam = true;
    return value;
}
