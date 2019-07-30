
#ifndef GAMECONTROLLER_AXIS_H
#define GAMECONTROLLER_AXIS_H

#include <Servo.h>
#include <PID_v1.h>

class Axis {

private:
    Servo xServo;
    int servoPort;
    double Kp = .2, Ki = .1, Kd = .0;
    PID myPID;
    int serialbnoAnglePrefix = -1;
    int serialSpeedAdjustedPrefix = -1;

public:
    Axis(int servoPort,
         int controllerDirection,
         int serialbnoAnglePrefix,
         int serialSpeedAdjustedPrefix
    );

    void update();

    void setup();

    double setpointAngle = 0;
    double bnoAngle = 0;
    double calibratedAngle = 0;
    double calibration = 0;
    double xSpeed = 0;
    double threshold = .15;
    double speedAdjusted = 0;
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
