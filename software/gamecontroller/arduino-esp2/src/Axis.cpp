
#include "Util.h"
#include "Axis.h"

Axis::Axis(int _servoPort, int controllerDirection, int _serialbnoAnglePrefix, int _serialSpeedAdjustedPrefix) :
        myPID(&calibratedAngle, &speed, &setpointAngle, Kp, Ki, Kd, controllerDirection),
        serialbnoAnglePrefix(_serialbnoAnglePrefix),
        serialSpeedAdjustedPrefix(_serialSpeedAdjustedPrefix)
        {

    servoPort = _servoPort;
    myPID.SetOutputLimits(-70, 70);
    myPID.SetMode(AUTOMATIC);
    myPID.SetSampleTime(20);
}


void Axis::setup() {
    servo.attach(servoPort);
    servo.write(90);
}

void Axis::update() {

    calibratedAngle = bnoAngle + calibration;

    myPID.Compute();

    if ((speed > threshold) || (speed < 0 - threshold)) {
        if (speed > 0) {
            speedAdjusted = speed + xMinSpeed;
        }
        if (speed < 0) {
            speedAdjusted = speed - xMinSpeed;
        }
    } else {
        speedAdjusted = 0;
    }
    servo.write(90 - speedAdjusted);

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

