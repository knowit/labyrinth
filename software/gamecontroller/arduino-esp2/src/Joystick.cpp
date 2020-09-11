
#include "Joystick.h"

static const int X_PORT = 34;
static const int Y_PORT = 35;

int joystickXCalibration = 0;
int joystickYCalibration = 0;

Joystick::Joystick() {
}

int Joystick::setup() {
    pinMode(X_PORT, INPUT);
    pinMode(Y_PORT, INPUT);
    return 0;
}

void Joystick::calibrate() {
    delay(500);
    joystickXCalibration = 2048 - analogRead(X_PORT);
    joystickYCalibration = 2048 - analogRead(Y_PORT);
}

void Joystick::update() {

    x = (analogRead(X_PORT) + joystickXCalibration) / 4095.0 * -1.0;
    y = (analogRead(Y_PORT) + joystickYCalibration) / 4095.0;

    /*
    Serial.print("Reading joystick. X=");
    Serial.print(analogRead(X_PORT));
    Serial.print("/");
    Serial.print(x);
    Serial.print(", Y=");
    Serial.print(analogRead(Y_PORT));
    Serial.print("/");
    Serial.print(y);
    Serial.println();
*/

}

