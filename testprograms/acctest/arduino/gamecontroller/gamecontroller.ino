
#include <SoftwareSerial.h>

SoftwareSerial logSerial(10, 11); // RX, TX

void setup() {

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  logSerial.begin(9600);

  logSerial.println("--SETUP--");


  logSerial.println("--SETUP DONE --");
  
}

void loop() { 

  

}
