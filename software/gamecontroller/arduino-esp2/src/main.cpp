#include <Arduino.h>
// #include <SoftwareSerial.h>
#include <SPI.h>
#include <TaskScheduler.h>
#include <Wire.h>
// #include <utility/imumaths.h>
#include "Axis.h"
#include "Util.h"
#include "BNOReader.h"
#include "SSD1306Display.h"
#include "Joystick.h"
#include <ESPmDNS.h>
#include <Preferences.h>
#include <wificonfigmode.h>

Axis xAxis(4, 1, 1, 2);
Axis yAxis(19, 0, 3, 4);
BNOReader bnoReader;
SSD1306Display display(xAxis, yAxis);
Joystick joystick;
Preferences preferences;

WebServer webServer(80);

void addAndEnableTask(Task &task);

double handle2ByteFloatParameter(const char *parameterName, int minValue, int maxValue, float offset);

void updateDisplay() {
    display.updateDisplay();
}

Task updateDisplayTask(200, TASK_FOREVER, &updateDisplay);


void readJoystick() {
    joystick.update();
}

Task readJoystickTask(10, TASK_FOREVER, &readJoystick);

void serveWebRequests() {
    webServer.handleClient();
}

Task serveWebRequestsTask(100, TASK_FOREVER, &serveWebRequests);

Scheduler runner;

void setupTasks() {
    runner.init();
    addAndEnableTask(updateDisplayTask);
    addAndEnableTask(readJoystickTask);
    addAndEnableTask(serveWebRequestsTask);
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

void setupWIFI() {

    delay(1000);

    preferences.begin("dings01", false);
    String name = preferences.getString("name", String("not set"));
    String ssid = preferences.getString("ssid", String("not set"));
    String pwd = preferences.getString("pwd", String("not set"));
    preferences.end();

    Serial.print("pwd=");
    Serial.print(pwd);

    Serial.print("name=");
    Serial.print(name);
    Serial.print(" , ");
    Serial.print("ssid=");
    Serial.print(ssid);
    Serial.print(" , ");
    Serial.print("pwd=");
    Serial.print(pwd);
    Serial.print(" , ");
    Serial.print("rport=");
    Serial.print(preferences.getInt("rport", -1));
    Serial.print(" , ");
    Serial.print("sport=");
    Serial.print(preferences.getInt("sport", -1));
    Serial.print(" , ");

    Serial.print("Connecting to WiFi. ssid=");
    Serial.println(ssid);

    if (pwd == "none") {
        Serial.println("Using no password");
        WiFi.begin(
                ssid.c_str(), ""
        );
    } else {
        WiFi.begin(
                ssid.c_str(),
                pwd.c_str()
        );
    }

    int timeout = 0;
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(250);
        delay(250);
        timeout++;
        if (timeout > 20) {
            Serial.println("Unable to connect to network (SSID="+ssid+").");
            serve();
        }
    }

    Serial.print("Connecting to WiFi. IP=");
    Serial.println(WiFi.localIP());

    if (!MDNS.begin(name.c_str())) {
        Serial.println("Unable to set up MSDN");
        while (1) {
            delay(1000);
        }
    }

    Serial.print("MSDN setup OK. name=");
    Serial.print(name);
    Serial.println(".local");
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

    yAxis.calibration = -1.5;
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
    xAxis.setpointAngle = (joystick.x * 12) + 6;
    yAxis.setpointAngle = (joystick.y * 10) - 5;
    xAxis.update();
    yAxis.update();

}

void readAndParseCommands() {
    if (Serial.available() >= 3) {
        int cmd = Serial.read();
        /*
        if (cmd == 1) { // set x angle
            xAxis.setpointAngle = (map(read2byteFloat(), 0, 16000, 0, 100 * 100) / 100.0) - 50;
        }
        if (cmd == 11) { // set y angle
            yAxis.setpointAngle = (map(read2byteFloat(), 0, 16000, 0, 100 * 100) / 100.0) - 50;
        }
         */
        if (cmd == 2) {
            xAxis.setKp(handle2ByteFloatParameter("X Kp: ", 0, 100, 0));
        }
        if (cmd == 12) {
            yAxis.setKp(handle2ByteFloatParameter("Y Kp: ", 0, 100, 0));
        }
        if (cmd == 3) {
            xAxis.setKi(handle2ByteFloatParameter("X Ki: ", 0, 100, 0));
        }
        if (cmd == 13) {
            yAxis.setKi(handle2ByteFloatParameter("Y Ki: ", 0, 100, 0));
        }
        if (cmd == 4) {
            xAxis.setKd(handle2ByteFloatParameter("X Kd: ", 0, 100, 0));
        }
        if (cmd == 14) {
            yAxis.setKd(handle2ByteFloatParameter("Y Kd: ", 0, 100, 0));
        }
        if (cmd == 5) {
            xAxis.calibration = handle2ByteFloatParameter("X cal: ", 0, 100, -50.);
        }
        if (cmd == 15) {
            yAxis.calibration = handle2ByteFloatParameter("Y cal: ", 0, 100, -50.);
        }
    }
}

double handle2ByteFloatParameter(const char *parameterName, int minValue, int maxValue, float offset) {
    float t = read2byteFloat();
    double value = map(t, 0, 16000, minValue, maxValue * 100) / 100.0 + offset;
    display.task = parameterName;
    display.taskParam = value;
    display.showTaskParam = true;
    return value;
}
