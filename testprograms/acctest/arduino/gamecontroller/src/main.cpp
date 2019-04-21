#include <Arduino.h>
// #include <SoftwareSerial.h>
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
// SoftwareSerial logSerial(10, 11); // RX, TX
Adafruit_BNO055 bno = Adafruit_BNO055(0X01);

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

float xPosSpeedFactor = 1;

// PID
double Kp = 0, Ki = 0, Kd = .0;
// double Kp = 7, Ki = 4, Kd = .21;
double Setpoint, Input, Output;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

double xSpeed = 0;
double xThreshold = .15;
double xSpeedAdjusted = 0;
float xMinSpeed = 12;
int xManualSpeed = 90;


byte msb(int numberToSend) {
    return 64 + numberToSend / 192;
}

byte lsb(int numberToSend) {
    return 64 + numberToSend % 192;
}

void write2byteFloat(int pseudoValue) {
    Serial.write(msb(pseudoValue));
    Serial.write(lsb(pseudoValue));
}


void setSystemStateState(const char *newState);

void readAndParseCommands();

void addAndEnableTask(Task &task);

void reportXBno() {
    Serial.write(1);
    write2byteFloat(map(bnoX * 100, -90 * 100, 90 * 100, 0, 16000));
}

Task reportXBnoTask(200, TASK_FOREVER, &reportXBno);

void reportXSpeedAdjusted() {
    Serial.write(2);
    write2byteFloat(map(xSpeedAdjusted * 100, -90 * 100, 90 * 100, 0, 16000));
}

Task reportXSpeedAdjustedTask(200, TASK_FOREVER, &reportXSpeedAdjusted);


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
    display.println(xSpeedAdjusted);

    display.setCursor(xPosSet, 8 * 3);
    display.println(setYAngle);
    display.setCursor(xPosBno, 8 * 3);
    display.println(bnoY);

    display.display();
}

Task updateDisplayTask(200, TASK_FOREVER, &updateDisplay);

Scheduler runner;

void setupTasks() {
//    logSerial.print("init Task Scheduler ... ");
    runner.init();
    addAndEnableTask(updateDisplayTask);
    addAndEnableTask(reportXBnoTask);
    addAndEnableTask(reportXSpeedAdjustedTask);
//    logSerial.println("OK");
}


void setSystemStateState(const char *newState) {
    state = newState;
}

void setTask(const char *newTask) {
    task = newTask;
}

void setup() {
//    logSerial.begin(9600);
//    logSerial.println("---- SETUP_START");

    pinMode(2, INPUT_PULLUP);

//    logSerial.print("init display ... ");
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
//        logSerial.println(F("SSD1306 allocation failed"));
        for (;;); // Don't proceed, loop forever
    }
    setSystemStateState("init");
    updateDisplay();
//    logSerial.println("OK");

    setTask("init serial");
    updateDisplay();
    Serial.begin(115200);

    setTask("init bno");
    updateDisplay();
//    logSerial.print("init bno ... ");
    if (!bno.begin()) {
//        logSerial.println("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
        while (1);
    }
    delay(1000);
    bno.setExtCrystalUse(true);

    setTask("init BNO ..DONE");
    updateDisplay();
//    logSerial.println("OK");

//    logSerial.print("init servos ... ");
    xServo.attach(6);
    xServo.write(90);
//    logSerial.println("OK");

//    logSerial.print("init PID ... ");
    Input = 0;
    Setpoint = 0;
    myPID.SetOutputLimits(-70, 70);
    myPID.SetMode(AUTOMATIC);
    myPID.SetSampleTime(200);
//    logSerial.println("OK");

    setupTasks();

//    logSerial.println("---- SETUP_DONE");
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

    if ( (xSpeed > xThreshold) || (xSpeed  <  0 - xThreshold) )  {
        if (xSpeed > 0) {
            xSpeedAdjusted = xSpeed + xMinSpeed;
        }
        if (xSpeed < 0) {
            xSpeedAdjusted = xSpeed - xMinSpeed;
        }
    } else {
        xSpeedAdjusted = 0;
    }
    xServo.write(90 - xSpeedAdjusted);
}

void readAndParseCommands() {
    if (Serial.available() >= 3) {
        int cmd = Serial.read();
        if (cmd < 64) {
            if (cmd == 1) { // setAngle
                setXAngle = (map(read2byteFloat(), 0, 16000, 0, 100 * 100) / 100.0) - 50;
                float y = read2byteFloat();
                if (y != -999) {
                    setYAngle = map(y, 0, 16000, -90, 90);
                }
            }
            if (cmd == 2) { // set X Kp
                float t = read2byteFloat();
                Kp = map(t, 0, 16000, 0, 100 * 100) / 100.0;
                setTask("x Kp: ");
                taskParam = myPID.GetKp();
                myPID.SetTunings(Kp, Ki, Kd);
                showTaskParam = true;
            }
            if (cmd == 3) { // set X Ki
                float t = read2byteFloat();
                Ki = map(t, 0, 16000, 0, 100 * 100) / 100.0;
                setTask("x Ki: ");
                taskParam = Ki;
                myPID.SetTunings(Kp, Ki, Kd);
                showTaskParam = true;
            }
            if (cmd == 4) { // set X Kd
                float t = read2byteFloat();
                if (t >= 0) {
                    Kd = map(t, 0, 16000, 0, 100 * 100) / 100.0;
                    setTask("x Kd: ");
                    taskParam = Kd;
                    myPID.SetTunings(Kp, Ki, Kd);
                    showTaskParam = true;
                }
            }
            if (cmd == 5) { // set X pos speed factor
                float s = read2byteFloat();
                xPosSpeedFactor = map(s, 0, 16000, 0, 100 * 100) / 100.0;
                setTask("x pos spd f: ");
                taskParam = xPosSpeedFactor;
                showTaskParam = true;
            }
            if (cmd == 6) { // set X minimum speed
                float f = read2byteFloat();
                if (f >= 0) {
                    xMinSpeed = map(f, 0, 16000, 0, 100 * 100) / 100.0;
                    setTask("x min spd: ");
                    taskParam = xMinSpeed;
                    showTaskParam = true;
                }
            }
            if (cmd == 7) { // set X manual
                float f = read2byteFloat();
                if (f >= 0) {
                    xManualSpeed = map(f, 0, 16000, 0, 180 * 100) / 100.0;
                    setTask("x man spd: ");
                    taskParam = xManualSpeed;
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

