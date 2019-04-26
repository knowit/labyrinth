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

Axis xAxis(6);
BNOReader bnoReader;
SSD1306Display display(xAxis);

void readAndParseCommands();

void addAndEnableTask(Task &task);

double handle2ByteFloatParameter(const char *parameterName, int minValue, int maxValue);

void xAxisReportState() {
    xAxis.reportState();
}

Task xAxisReportStateTask(200, TASK_FOREVER, &xAxisReportState);

void updateDisplay() {
    display.updateDisplay();
}

Task updateDisplayTask(200, TASK_FOREVER, &updateDisplay);

Scheduler runner;

void setupTasks() {
    runner.init();
    addAndEnableTask(updateDisplayTask);
    addAndEnableTask(xAxisReportStateTask);
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
    xAxis.update();
}

void readAndParseCommands() {
    if (Serial.available() >= 3) {
        int cmd = Serial.read();
        if (cmd < 64) {
            if (cmd == 1) { // setAngle
                xAxis.setpointAngle = (map(read2byteFloat(), 0, 16000, 0, 100 * 100) / 100.0) - 50;
                float y = read2byteFloat();
            }
            if (cmd == 2) {
                double kp = handle2ByteFloatParameter("x Kp: ", 0, 100);
                if (kp > 0) {
                    xAxis.setKp(kp);
                }
            }
            if (cmd == 3) {
                double ki = handle2ByteFloatParameter("x Ki: ", 0, 100);
                if (ki > 0) {
                    xAxis.setKi(ki);
                }
            }
            if (cmd == 4) {
                double kd = handle2ByteFloatParameter("x Kd: ", 0, 100);
                if (kd > 0) {
                    xAxis.setKd(kd);
                }
            }
            if (cmd == 6) {
                double minSpeed = handle2ByteFloatParameter("x min spd: ", 0, 100);
                if (minSpeed > 0) {
                    xAxis.xMinSpeed = minSpeed;
                }
            }
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
