
#ifndef GAMECONTROLLER_AXIS_H
#define GAMECONTROLLER_AXIS_H

#include <Servo.h>

class Axis {

private:
    Servo xServo;
    int servoPort;

public:
    Axis(int servoPort);

    void update(int speed);
    void setup();
};


#endif
