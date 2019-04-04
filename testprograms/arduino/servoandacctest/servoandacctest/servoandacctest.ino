#include <Servo.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#define BNO055_SAMPLERATE_DELAY_MS (100)

Adafruit_BNO055 bno = Adafruit_BNO055(55);
Servo xServo;
Servo yServo;

int cmd = 0;



void setup() {
  Serial.begin(9600);
  xServo.attach(9);
  xServo.write(90);  // set servo to mid-point
  yServo.attach(8);
  yServo.write(90);  // set servo to mid-point

  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    // Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    // while(1);
  }

  delay(1000);

  bno.setExtCrystalUse(true);
}



void loop() {
  if (Serial.available() > 1) {
    cmd = Serial.read();
    // Serial.println(cmd);
    if ( cmd == 1 ) {
      yServo.write(Serial.read());
    }
    if ( cmd == 2 ) {
      xServo.write(Serial.read());
    }
  }

  sensors_event_t event;
  bno.getEvent(&event);

    /* Display the floating point data */
  Serial.write((byte)map(event.orientation.y, 90, -90, 0, 256));
  Serial.write((byte)map(event.orientation.z, 90, -90, 0, 256));


  
}
