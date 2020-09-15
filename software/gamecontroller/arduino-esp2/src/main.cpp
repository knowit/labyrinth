#include <Arduino.h>

#include "pb_common.h"
#include "pb.h"
#include "pb_encode.h"
#include "pb_decode.h"

#include "./pb/GameMessage.pb.h"

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
#include <WiFiUdp.h>
#include "AsyncUDP.h"

#include <Preferences.h>

#include <wificonfigmode.h>

Axis xAxis(4, 1, 1, 2);
Axis yAxis(19, 0, 3, 4);
WebServer webServer(80);
const char *udpAddress = "192.168.10.165";
BNOReader bnoReader;
SSD1306Display display(xAxis, yAxis);
Joystick joystick;
AsyncUDP asyncUdp;
Preferences preferences;
WiFiUDP udp;
uint8_t buffer[512];

const int udpPort = 4049;

void addAndEnableTask(Task &task);


void updateDisplay() {
    display.updateDisplay();
}

Task updateDisplayTask(200, TASK_FOREVER, &updateDisplay);


void readJoystick() {
    joystick.update();
}

Task readJoystickTask(10, TASK_FOREVER, &readJoystick);

void sendUDPMessage() {
    // Serial.println("sendUDPMessage");

    BoardState boardState = BoardState_init_zero;
    boardState.has_orientation=true;
    boardState.orientation.x =bnoReader.xAngle;
    boardState.orientation.y = bnoReader.yAngle;

    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));

    bool status = pb_encode(&stream, BoardState_fields, &boardState);

    if (!status) {
        Serial.println("Failed to encode");
        return;
    }

/*
    Serial.print("Message Length: ");
    Serial.println(stream.bytes_written);

    Serial.print("Message: ");

    for(int i = 0; i<stream.bytes_written; i++){
        Serial.printf("%02X",buffer[i]);
    }
    Serial.println();
*/


    //send hello world to server
    udp.beginPacket(udpAddress, udpPort);
    udp.write(buffer,stream.bytes_written );
    udp.endPacket();
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
    addAndEnableTask(readJoystickTask);
    addAndEnableTask(serveWebRequestsTask);
    addAndEnableTask(sendUDPMessageTask);
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

void setupUDP(){
    udp.begin(udpPort);
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

    Serial.println("setup setup BEGIN");
    display.task = "init UDP/PB";
    updateDisplay();
    setupUDP();
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

    if (asyncUdp.listen(4050)) {
        Serial.print("UDP Listening on IP: ");
        Serial.println(WiFi.localIP());
        asyncUdp.onPacket([](AsyncUDPPacket packet) {
            Serial.print("UDP Packet Type: ");
            Serial.print(packet.isBroadcast() ? "Broadcast" : packet.isMulticast() ? "Multicast" : "Unicast");
            Serial.print(", From: ");
            Serial.print(packet.remoteIP());
            Serial.print(":");
            Serial.print(packet.remotePort());
            Serial.print(", To: ");
            Serial.print(packet.localIP());
            Serial.print(":");
            Serial.print(packet.localPort());
            Serial.print(", Length: ");
            Serial.print(packet.length()); //dlzka packetu
            Serial.print(", Data: ");
            Serial.write(packet.data(), packet.length());

            Serial.print(", Message: ");
            for(int i = 0; i<sizeof(packet.data()); i++){
                Serial.printf("%02X",packet.data()[i]);
            }

            Serial.println();

            //Serial.println();
            //String myString = (const char *) packet.data();

            pb_istream_t stream = pb_istream_from_buffer(packet.data(), packet.length());

            JoystickState message = JoystickState_init_zero;
            if (!pb_decode(&stream, JoystickState_fields, &message)) {
                Serial.println("Failed to decode");
                return;
            }

            Serial.println(message.orientation.x);
            Serial.println(message.orientation.y);

            xAxis.setpointAngle = message.orientation.x;
            yAxis.setpointAngle = message.orientation.y;

        });
    }

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
    // xAxis.setpointAngle = (joystick.x * 12) + 6;
    // yAxis.setpointAngle = (joystick.y * 10) - 5;
    xAxis.update();
    yAxis.update();

}


