#include <Arduino.h>
#include <SparkFun_MAG3110.h>

MAG3110 mag = MAG3110(); //Instantiate MAG3110

void printValue(int value);

int adjust(int rawValue);

void setup() {


    delay(2000);

    Serial.begin(115200);

    Wire.begin();             //setup I2C bus
    Wire.setClock(400000);    // I2C fast mode, 400kHz

    mag.initialize(); //Initializes the mag sensor
    mag.start();      //Puts the sensor in active mode
}

void loop() {

    int x, y, z;
    int xAdj, yAdj, zAdj;
    //Only read data when it's ready
    if (mag.dataReady()) {
        //Read the data
        mag.readMag(&x, &y, &z);

        xAdj = adjust(x);
        printValue(xAdj);

        yAdj = adjust(y);
        printValue(yAdj);

        zAdj = adjust(z);
        printValue(zAdj);

        printValue(max(xAdj, max(yAdj, zAdj)));

        Serial.println();
    }
}

void printValue(int value) {
    Serial.print(value);
    Serial.print(",");
}

int adjust(int rawValue) { return rawValue > 32767 ? rawValue - (32767 * 2) : rawValue; }