
#ifndef GAMECONTROLLER_AXIS_H
#define GAMECONTROLLER_AXIS_H

#include <Servo.h>
#include <PID_v1.h>

class Axis {

private:
    Servo xServo;
    int servoPort;

public:
    Axis(int servoPort);

    void update();
    void setup();
    float setpointAngle = 0;
    float bnoAngle = 0;
    double xSpeed = 0;
    double xThreshold = .15;
    double xSpeedAdjusted = 0;
    float xMinSpeed = 0;
    int xManualSpeed = 90;
    // PID
    double Kp = 0, Ki = 0, Kd = .0;
    double Setpoint, Input, Output;
    PID myPID;

};


#endif
