//
// Created by Andreas Rosnes Bade on 2019-04-21.
//

#include "Axis.h"
#include <SoftwareSerial.h>


Axis::Axis(int _servoPort) :
        myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT) {
    servoPort = _servoPort;
    Input = 0;
    Setpoint = 0;
    myPID.SetOutputLimits(-70, 70);
    myPID.SetMode(AUTOMATIC);
    myPID.SetSampleTime(200);
}


void Axis::setup() {
    xServo.attach(servoPort);
    xServo.write(90);
}

void Axis::update() {

    Input = bnoAngle;
    Setpoint = setpointAngle;

    myPID.Compute();
    xSpeed = Output;

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
