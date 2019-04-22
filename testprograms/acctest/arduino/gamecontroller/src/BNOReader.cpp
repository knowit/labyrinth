//
// Created by Andreas Rosnes Bade on 2019-04-22.
//

#include "BNOReader.h"

void BNOReader::setup() {

    if (!bno.begin()) {
        // No BNO055 detected ... Check your wiring or I2C ADDR!
        while (1);
    }
    delay(1000);
    bno.setExtCrystalUse(true);

}

void BNOReader::update() {
    sensors_event_t event;
    bno.getEvent(&event);

    xAngle = event.orientation.y;
    yAngle = event.orientation.z;

}