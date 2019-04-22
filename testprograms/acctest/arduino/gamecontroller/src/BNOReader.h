
#ifndef GAMECONTROLLER_BNOREADER_H
#define GAMECONTROLLER_BNOREADER_H

#define BNO055_SAMPLERATE_DELAY_MS (20)

#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>


class BNOReader {

private:
    Adafruit_BNO055 bno = Adafruit_BNO055(0X01);

public:
    void setup();
    double xAngle = 0;
    double yAngle = 0;
    void update();

};


#endif //GAMECONTROLLER_BNOREADER_H
