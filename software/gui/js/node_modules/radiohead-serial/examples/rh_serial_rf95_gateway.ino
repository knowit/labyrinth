// Example arduino sketch for using an arduino as a serial gateway.
// This gateway allows using any radio supported by the RadioHead library
// with the Node.js module 'radiohead-serial'.
//
// This example is taken from the original RadioHead examples.
// see http://www.airspayce.com/mikem/arduino/RadioHead/examples.html
//
// This sketch can be used as a gateway between 2 RadioHead radio networks (connected by a serial line),
// or between
// 1 RadioHead radio network and a Unix host.
// It relays all messages received on the radio driver to the serial port
// (using the RH_Serial driver and protocol). And it relays all messages received on the RH_Serial
// driver port to the radio driver.
// Both drivers operate in promiscuous mode and preserve all headers, so this sketch acts as
// a transparent gateway between RH_Serial and and other RadioHead driver.
//
// By replacing RH_Serial with another RadioHead driver, this can act as a universal gateway
// between any 2 RadioHead networks.
//
#include <SPI.h>
#include <RH_RF95.h>
#include <RH_Serial.h>
#include <RHReliableDatagram.h>

// Singleton instance of the radio driver
// You can use other radio drivers if you want
RH_RF95 radio;
//RH_RF95 radio(5, 2); // Rocket Scream Mini Ultra Pro with the RFM95W
//RH_RF95 radio(8, 3); // Adafruit Feather M0 with RFM95

// Singleton instance of the serial driver which relays all messages
// via Serial to another RadioHead RH_Serial driver, perhaps on a Unix host.
// You could use a different Serial if the arduino has more than 1, eg Serial1 on a Mega
RH_Serial serial(Serial);


void setup()
{
  // Rocket Scream Mini Ultra Pro with the RFM95W only:
  // Ensure serial flash is not interfering with radio communication on SPI bus
  //  pinMode(4, OUTPUT);
  //  digitalWrite(4, HIGH);

  Serial.begin(9600);
  if (!radio.init())
    Serial.println("radio init failed");
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on.
  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then
  // you can set transmitter powers from 5 to 23 dBm:
  //  radio.setTxPower(23, false);
  // If you are using Modtronix inAir4 or inAir9,or any other module which uses the
  // transmitter RFO pins and not the PA_BOOST pins
  // then you can configure the power transmitter power for -1 to 14 dBm and with useRFO true.
  // Failure to do that will result in extremely low transmit powers.
  //  radio.setTxPower(14, true);

  radio.setPromiscuous(true);

  if (!serial.init())
    Serial.println("serial init failed");

  serial.setPromiscuous(true);

}

uint8_t buf[RH_SERIAL_MAX_MESSAGE_LEN];

void loop()
{
  if (radio.available())
  {
    uint8_t len = sizeof(buf);
    radio.recv(buf, &len);

    serial.setHeaderTo(radio.headerTo());
    serial.setHeaderFrom(radio.headerFrom());
    serial.setHeaderId(radio.headerId());
    serial.setHeaderFlags(radio.headerFlags(), 0xFF); // Must clear all flags
    serial.send(buf, len);
  }

  if (serial.available())
  {
    uint8_t len = sizeof(buf);
    serial.recv(buf, &len);

    // Optional check the adress range.
    // Uncomment the following lines to only submit messages addressed to 200 or higher.
    // if(serial.headerTo() < 200){
    //  return;
    // }

    radio.setHeaderTo(serial.headerTo());
    radio.setHeaderFrom(serial.headerFrom());
    radio.setHeaderId(serial.headerId());
    radio.setHeaderFlags(serial.headerFlags(), 0xFF); // Must clear all flags
    radio.send(buf, len);
  }
}
