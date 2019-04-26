
#ifndef GAMECONTROLLER_AXIS_H
#define GAMECONTROLLER_AXIS_H

#include <Servo.h>
#include <PID_v1.h>

class Axis {

private:
    Servo xServo;
    int servoPort;
    double Kp = 2.5, Ki = 0, Kd = .0;
    PID myPID;

public:
    Axis(int servoPort);

    void update();
    void setup();
    double setpointAngle = 0;
    double bnoAngle = 0;
    double xSpeed = 0;
    double xThreshold = .15;
    double xSpeedAdjusted = 0;
    double xMinSpeed = 0;
    int xManualSpeed = 90;

    void setKp(double v);
    double GetKp();
    void setKi(double v);
    double GetKi();
    void setKd(double v);
    double GetKd();

    void reportState();

};


#endif
