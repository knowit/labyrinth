
#include <Wire.h>
#include "Axis.h"
Axis xAxis(4, 1, 1, 2);
Axis yAxis(19, 0, 3, 4);

#include "SSD1306Display.h"
SSD1306Display display(xAxis, yAxis);


#include "./Network.h"

#include <TaskScheduler.h>
#include "Util.h"
#include "BNOReader.h"
#include "Joystick.h"
#include "ISLPJoystickReceiver.h"
#include "ISLPBoardStateSender.h"


WebServer webServer(80);

BNOReader bnoReader;

Joystick joystick;

ISLPJoystickReceiver islpJoystickReceiver;
ISLPBoardStateSender islpBoardStateSender;

void addAndEnableTask(Task &task);

void resetSettings() {
    if( digitalRead(0) == 0) {
        resetPreferences();
        display.task = "Settings reset";
        Serial.println("Settings reset");
        display.updateDisplay();
        sleep(1000);
        ESP.restart();
    }
}

Task resetSettingsTask(1000, TASK_FOREVER, &resetSettings);

void updateDisplay() {
    display.updateDisplay();
}

Task updateDisplayTask(200, TASK_FOREVER, &updateDisplay);

void readJoystick() {
    joystick.update();
}

Task readJoystickTask(10, TASK_FOREVER, &readJoystick);

void sendUDPMessage() {
    islpBoardStateSender.update();
}

Task sendUDPMessageTask(10, TASK_FOREVER, &sendUDPMessage);

void serveWebRequests() {
    webServer.handleClient();
}

Task serveWebRequestsTask(100, TASK_FOREVER, &serveWebRequests);

Scheduler runner;

void setupTasks() {
    runner.init();
    addAndEnableTask(updateDisplayTask);
    // addAndEnableTask(readJoystickTask);
    addAndEnableTask(serveWebRequestsTask);
    addAndEnableTask(sendUDPMessageTask);
    addAndEnableTask(resetSettingsTask);
}


void calibrateaxis() {

    Serial.print("x=");
    float xCal = webServer.arg("x").toFloat();
    Serial.print(xCal);
    Serial.print("y=");
    float yCal = webServer.arg("y").toFloat();
    Serial.print(yCal);
    Serial.println();

    xAxis.calibration = xCal;
    yAxis.calibration = yCal;

    webServer.send(200, "text/html", "Hello");
}

void calibratejoystick() {

    Serial.println("Calibrate joystick - START");
    joystick.calibrate();
    Serial.println("Calibrate joystick - END");

    webServer.send(200, "text/html", "Joystick calibrated");
}

void setupServer() {
    webServer.on("/calibrateaxis", calibrateaxis);
    webServer.on("/calibratejoystick", calibratejoystick);
    webServer.begin();
}

void setup() {

    int displaySetupResult = display.setup();
    if (displaySetupResult != 0) {
        for (;;);
    }

    display.task = "init serial";
    Serial.begin(115200);
    delay(1000);
    Serial.println("____SETUP_BEGIN____");
    updateDisplay();


    display.state = "init";
    updateDisplay();

    display.task = "init wifi";
    updateDisplay();
    setupWIFI();

    Serial.println("setup setup BEGIN");
    display.task = "init UDP/PB";
    updateDisplay();

    display.task = "init UDP/PB OK";
    updateDisplay();
    Serial.println("setup UDP - OK");

    Serial.println("joystick setup BEGIN");
    display.task = "init joystick";
    updateDisplay();
    joystick.setup();
    joystick.calibrate();
    Serial.println("joystick setup END");

    display.task = "init web";
    Serial.println("Web setup");
    updateDisplay();
    setupServer();
    display.task = "init web - OK";
    updateDisplay();
    Serial.println("Web setup - OK");


    display.task = "init BNO";
    updateDisplay();
    int bnoSetupResult = bnoReader.setup();
    if (bnoSetupResult != 0) {
        display.task = "init BNO ERROR!";
        updateDisplay();
        for (;;);
    }
    display.task = "init BNO - OK";
    updateDisplay();

    Serial.println("Servo setup");
    xAxis.setup();
    yAxis.setup();

    Serial.println("Task setup");
    setupTasks();

    display.state = "Running";
    display.task = "";

    Serial.println("____SETUP_END______");

    islpJoystickReceiver.setup(4050);
    islpBoardStateSender.setup();

    yAxis.calibration = 0;
    xAxis.calibration = -.5;
}

void addAndEnableTask(Task &task) {
    runner.addTask(task);
    task.enable();
}

void loop() {
    runner.execute();

    bnoReader.update();

    xAxis.bnoAngle = bnoReader.xAngle;
    yAxis.bnoAngle = bnoReader.yAngle;
    // xAxis.setpointAngle = (joystick.x * 12) + 6;
    // yAxis.setpointAngle = (joystick.y * 10) - 5;./scr
    xAxis.setpointAngle = islpJoystickReceiver.x;
    yAxis.setpointAngle = islpJoystickReceiver.y;
    islpBoardStateSender.x =bnoReader.yAngle;
    islpBoardStateSender.y = bnoReader.xAngle;
    xAxis.update();
    yAxis.update();

}


