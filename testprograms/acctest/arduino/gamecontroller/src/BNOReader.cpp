//
// Created by Andreas Rosnes Bade on 2019-04-22.
//

#include "BNOReader.h"

int BNOReader::setup() {

    if (!bno.begin()) {
        Serial1.println("init BNO - ERROR: No BNO055 detected ... Check your wiring or I2C ADDR!");
        return 1;
    }
    Serial1.println("init BNO: Waiting 1 sec");
    delay(1000);
    bno.setExtCrystalUse(true);

    return 0;
}

void BNOReader::update() {
    sensors_event_t event;
    bno.getEvent(&event);

    xAngle = event.orientation.y;
    yAngle = event.orientation.z;

}