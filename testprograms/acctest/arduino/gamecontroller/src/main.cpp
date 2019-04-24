#include <Arduino.h>
// #include <SoftwareSerial.h>
#include <SPI.h>
#include <TaskScheduler.h>
#include <Wire.h>
#include <Wire.h>
#include <utility/imumaths.h>
#include "Axis.h"
#include "BNOReader.h"
#include "SSD1306Display.h"


float read2byteFloat();

Axis xAxis(6);
BNOReader bnoReader;
SSD1306Display display(xAxis);

void readAndParseCommands();

void addAndEnableTask(Task &task);


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
    if( displaySetupResult != 0) {
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
                if (y != -999) {
                    setYAngle = map(y, 0, 16000, -90, 90);
                }
            }
            if (cmd == 2) { // set X Kp
                float t = read2byteFloat();
                xAxis.setKp(map(t, 0, 16000, 0, 100 * 100) / 100.0);
                display.task = "x Kp: ";
                display.taskParam = xAxis.GetKp();
                display.showTaskParam = true;
            }
            if (cmd == 3) { // set X Ki
                float t = read2byteFloat();
                xAxis.setKi(map(t, 0, 16000, 0, 100 * 100) / 100.0);
                display.task = "x Ki: ";
                display.taskParam = xAxis.GetKi();
                display.showTaskParam = true;
            }
            if (cmd == 4) { // set X Kd
                float t = read2byteFloat();
                if (t >= 0) {
                    xAxis.setKd(map(t, 0, 16000, 0, 100 * 100) / 100.0);
                    display.task = "x Kd: ";
                    display.taskParam = xAxis.GetKd();
                    display.showTaskParam = true;
                }
            }
            if (cmd == 6) { // set X minimum speed
                float f = read2byteFloat();
                if (f >= 0) {
                    xAxis.xMinSpeed = map(f, 0, 16000, 0, 100 * 100) / 100.0;
                    display.task = "x min spd: ";
                    display.taskParam = xAxis.xMinSpeed;
                    display.showTaskParam = true;
                }
            }
            if (cmd == 7) { // set X manual
                float f = read2byteFloat();
                if (f >= 0) {
                    xAxis.xManualSpeed = map(f, 0, 16000, 0, 180 * 100) / 100.0;
                    display.task = "x man spd: ";
                    display.taskParam = xAxis.xManualSpeed;
                    display.showTaskParam = true;
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

