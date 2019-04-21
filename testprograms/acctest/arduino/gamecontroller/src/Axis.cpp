//
// Created by Andreas Rosnes Bade on 2019-04-21.
//

#include "Axis.h"
#include <SoftwareSerial.h>


Axis::Axis(int _servoPort) {
    servoPort = _servoPort;
}

void Axis::setup() {
    xServo.attach(servoPort);
    xServo.write(90);
}

void Axis::update(int speed) {
    xServo.write(speed);
}