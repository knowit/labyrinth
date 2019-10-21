#include <Arduino.h>
#include <SparkFun_MAG3110.h>

MAG3110 mag0 = MAG3110(0); //Instantiate MAG3110
MAG3110 mag1 = MAG3110(1); //Instantiate MAG3110

void printValue(int value);

int adjust(int rawValue);

void setup() {


    delay(2000);

    Serial.begin(115200);

    Wire.begin();
    Wire.setClock(400000);    // I2C fast mode, 400kHz

    mag0.initialize(); //Initializes the mag sensor
    mag0.start();      //Puts the sensor in active mode

    Wire1.begin(33, 32, 400000);
    Wire1.setClock(400000);    // I2C fast mode, 400kHz

    mag1.initialize(); //Initializes the mag sensor
    mag1.start();      //Puts the sensor in active mode
}

void loop() {

    int x0, y0, z0;
    int x1, y1, z1;

    if (mag0.dataReady() && mag1.dataReady()) {

        mag1.readMag(&x0, &y0, &z0);
        mag0.readMag(&x1, &y1, &z1);

        printValue(adjust(x0));
        printValue(adjust(y0));
        printValue(adjust(z0));

        printValue(adjust(x1));
        printValue(adjust(y1));
        printValue(adjust(z1));

        Serial.println();
    }
}

void printValue(int value) {
    Serial.print(value);
    Serial.print(",");
}

int adjust(int rawValue) { return rawValue > 32767 ? rawValue - (32767 * 2) : rawValue; }