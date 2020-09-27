
#include <Arduino.h>


#ifndef GAMECONTROLLER_JOYSTICK_H
#define GAMECONTROLLER_JOYSTICK_H

#include <WiFi.h>

class Joystick {

private:

public:

    Joystick();

    void update();
    void calibrate();
    int setup();

    double x;
    double y;


};


#endif //GAMECONTROLLER_JOYSTICK_H
