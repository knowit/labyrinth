#include <Arduino.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <PID_v1.h>
#include <TaskScheduler.h>

#define BNO055_SAMPLERATE_DELAY_MS (20)

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
SoftwareSerial logSerial(10, 11); // RX, TX
Adafruit_BNO055 bno = Adafruit_BNO055(55);

String state = "unknow";
String task = "";
bool showTaskParam = false;
float taskParam = 0;
float setXAngle = 0;
float bnoX = 0;
Servo xServo;
float setYAngle = 0;
float bnoY = 0;

float read2byteFloat();

float xThreshold = .1;
float xSpeedFactor = 5;
int xMaxSpeed = 69;
int xMinSpeed = 18;

// PID
//double Kp = 2, Ki = 5, Kd = 1;
double Kp = 7, Ki = 4, Kd = .21;
double Setpoint, Input, Output;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

double xSpeed = 0;

void setSystemStateState(const char *newState);

void readAndParseCommands();

void addAndEnableTask(Task &task);

Scheduler runner;

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
    display.println(setXAngle);
    display.setCursor(xPosBno, 8 * 2);
    display.println(bnoX);
    display.setCursor(xPosSpeed, 8 * 2);
    display.println(xSpeed);

    display.setCursor(xPosSet, 8 * 3);
    display.println(setYAngle);
    display.setCursor(xPosBno, 8 * 3);
    display.println(bnoY);

    display.display();
}


void t1Callback() {
    updateDisplay();
}
Task updateDisplayTask(200, TASK_FOREVER, &t1Callback);

void setSystemStateState(const char *newState) {
    state = newState;
}

void setTask(const char *newTask) {
    task = newTask;
}


void setup() {
    logSerial.begin(9600);
    logSerial.println("---- SETUP_START");

    pinMode(2, INPUT_PULLUP);

    logSerial.print("init display ... ");
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
        logSerial.println(F("SSD1306 allocation failed"));
        for (;;); // Don't proceed, loop forever
    }
    setSystemStateState("init");
    updateDisplay();
    logSerial.println("OK");

    setTask("init serial");
    updateDisplay();
    Serial.begin(115200);

    setTask("init bno");
    updateDisplay();
    logSerial.print("init bno ... ");
    if (!bno.begin()) {
        logSerial.println("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
        while (1);
    }
    delay(1000);
    bno.setExtCrystalUse(true);
    setTask("init BNO ..DONE");
    updateDisplay();
    logSerial.println("OK");

    logSerial.print("init servos ... ");
    xServo.attach(6);
    xServo.write(90);
    logSerial.println("OK");

    logSerial.print("init PID ... ");
    Input = 0;
    Setpoint = 0;
    myPID.SetOutputLimits(-90,90);
    myPID.SetMode(AUTOMATIC);
    logSerial.println("OK");

    logSerial.print("init Task Scheduler ... ");
    runner.init();
    addAndEnableTask(updateDisplayTask);
    logSerial.println("OK");

    logSerial.println("---- SETUP_DONE");
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

    sensors_event_t event;
    bno.getEvent(&event);

    if (bnoX != event.orientation.y || bnoY != event.orientation.z) {
        bnoX = event.orientation.y;
        bnoY = event.orientation.z;

        Input = bnoX;
    }

    Setpoint = setXAngle;

    myPID.Compute();
    xSpeed = Output;

    if( xSpeed < -89 ) xSpeed = -89;
    if( xSpeed > 89 ) xSpeed = 89;

    xServo.write(90 + xSpeed);

}

void readAndParseCommands() {
    if (Serial.available() >= 3)  {
        int cmd = Serial.read();
        if (cmd < 64) {
            if (cmd == 1) { // setAngle
                float x = read2byteFloat();
                if (x != -999) {
                    setXAngle = map(x, 0, 16000, -90 * 100, 90 * 100) / 100.0;
                }
                float y = read2byteFloat();
                if (y != -999) {
                    setYAngle = map(y, 0, 16000, -90 * 100, 90 * 100) / 100.0;
                }
            }
            if (cmd == 2) { // setXThreshold
                float t = read2byteFloat();
                if (t != -999) {
                    xThreshold = map(t, 0, 16000, 0, 10 * 100) / 100.0;
                }
                setTask("x threshold: ");
                taskParam = xThreshold;
                showTaskParam = true;
            }
            if (cmd == 3) { // setXSpeedFactor
                float s = read2byteFloat();
                if (s != -999) {
                    xSpeedFactor = map(s, 0, 16000, 0, 90 * 100) / 100.0;
                }
                setTask("x spd fct: ");
                taskParam = xSpeedFactor;
                showTaskParam = true;
            }
            if (cmd == 4) { // setXMaxSpeed
                float s = read2byteFloat();
                if (s != -999) {
                    xMaxSpeed = map(s, 0, 16000, 0, 90 * 100) / 100.0;
                }
                setTask("x max spd: ");
                taskParam = xMaxSpeed;
                showTaskParam = true;
            }
            if (cmd == 5) { // setXMinSpeed
                float s = read2byteFloat();
                if (s != -999) {
                    xMinSpeed = map(s, 0, 16000, 0, 90 * 100) / 100.0;
                }
                setTask("x min spd: ");
                taskParam = xMinSpeed;
                showTaskParam = true;
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

