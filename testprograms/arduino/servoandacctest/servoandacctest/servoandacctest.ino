#include <Servo.h>

Servo xServo;
Servo yServo;

int cmd = 0;

void setup() {
  Serial.begin(9600);
  xServo.attach(9);
  xServo.write(90);  // set servo to mid-point
  yServo.attach(8);
  yServo.write(90);  // set servo to mid-point
}



void loop() {
  if (Serial.available() > 1) {
    cmd = Serial.read();
    Serial.println(cmd);
    if ( cmd == 1 ) {
      yServo.write(Serial.read());
    }
    if ( cmd == 2 ) {
      xServo.write(Serial.read());
    }
  }
}
