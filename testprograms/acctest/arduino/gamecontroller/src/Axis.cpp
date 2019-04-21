//
// Created by Andreas Rosnes Bade on 2019-04-21.
//

#include "Axis.h"
#include <SoftwareSerial.h>


Axis::Axis(int _servoPort) :
        myPID(&bnoAngle, &xSpeed, &setpointAngle, Kp, Ki, Kd, DIRECT) {
    servoPort = _servoPort;
    myPID.SetOutputLimits(-70, 70);
    myPID.SetMode(AUTOMATIC);
    myPID.SetSampleTime(200);
}


void Axis::setup() {
    xServo.attach(servoPort);
    xServo.write(90);
}

void Axis::update() {

    myPID.Compute();

    if ((xSpeed > xThreshold) || (xSpeed < 0 - xThreshold)) {
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

void Axis::setKp(double v) {
    Kp = v;
    myPID.SetTunings(Kp, Ki, Kd);
}

double Axis::GetKp() {
    return myPID.GetKp();
}

void Axis::setKi(double v) {
    Ki = v;
    myPID.SetTunings(Kp, Ki, Kd);
}

double Axis::GetKi() {
    return myPID.GetKi();
}

void Axis::setKd(double v) {
    Kd = v;
    myPID.SetTunings(Kp, Ki, Kd);
}

double Axis::GetKd() {
    return myPID.GetKd();
}
