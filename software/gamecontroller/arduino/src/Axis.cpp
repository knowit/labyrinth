
#include "Util.h"
#include "Axis.h"

Axis::Axis(int _servoPort, int controllerDirection, int _serialbnoAnglePrefix, int _serialSpeedAdjustedPrefix) :
        myPID(&calibratedAngle, &xSpeed, &setpointAngle, Kp, Ki, Kd, controllerDirection),
        serialbnoAnglePrefix(_serialbnoAnglePrefix),
        serialSpeedAdjustedPrefix(_serialSpeedAdjustedPrefix)
        {

    servoPort = _servoPort;
    myPID.SetOutputLimits(-70, 70);
    myPID.SetMode(AUTOMATIC);
    myPID.SetSampleTime(100);
}


void Axis::setup() {
    xServo.attach(servoPort);
    xServo.write(90);
}

void Axis::update() {

    calibratedAngle = bnoAngle + calibration;

    myPID.Compute();

    if ((xSpeed > threshold) || (xSpeed < 0 - threshold)) {
        if (xSpeed > 0) {
            speedAdjusted = xSpeed + xMinSpeed;
        }
        if (xSpeed < 0) {
            speedAdjusted = xSpeed - xMinSpeed;
        }
    } else {
        speedAdjusted = 0;
    }
    xServo.write(90 - speedAdjusted);

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

void Axis::reportState() {
    Serial.write(serialbnoAnglePrefix);
    write2byteFloat(map(bnoAngle * 100, -90 * 100, 90 * 100, 0, 16000));

    Serial.write(serialSpeedAdjustedPrefix);
    write2byteFloat(map(speedAdjusted * 100, -90 * 100, 90 * 100, 0, 16000));

}
