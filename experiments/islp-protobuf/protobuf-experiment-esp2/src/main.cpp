#include <Arduino.h>

#include "test.pb.h"

#include "pb_common.h"
#include "pb.h"
#include "pb_encode.h"
#include "pb_decode.h"

#include <WiFi.h>
#include <WiFiUdp.h>
#include "AsyncUDP.h"

const char *ssid = "Bache";
const char *password = "71073826";

WiFiUDP udp;
AsyncUDP asyncUdp;


const char *udpAddress = "192.168.10.167";
const int udpPort = 4049;
uint8_t buffer[128];
int i;
TestMessage message = TestMessage_init_zero;

void setup() {

    Serial.begin(115200);

    WiFi.begin(ssid, password);
    Serial.println("Connecting");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());
    udp.begin(udpPort);


    if (asyncUdp.listen(4049)) {
        Serial.print("UDP Listening on IP: ");
        Serial.println(WiFi.localIP());
        asyncUdp.onPacket([](AsyncUDPPacket packet) {
            Serial.print("UDP Packet Type: ");
            Serial.print(packet.isBroadcast() ? "Broadcast" : packet.isMulticast() ? "Multicast" : "Unicast");
            Serial.print(", From: ");
            Serial.print(packet.remoteIP());
            Serial.print(":");
            Serial.print(packet.remotePort());
            Serial.print(", To: ");
            Serial.print(packet.localIP());
            Serial.print(":");
            Serial.print(packet.localPort());
            Serial.print(", Length: ");
            Serial.print(packet.length()); //dlzka packetu
            Serial.print(", Data: ");
            Serial.write(packet.data(), packet.length());

            Serial.print(", Message: ");
            for(int i = 0; i<sizeof(packet.data()); i++){
                Serial.printf("%02X",packet.data()[i]);
            }

            Serial.println();

            //Serial.println();
            //String myString = (const char *) packet.data();

            pb_istream_t stream = pb_istream_from_buffer(packet.data(), packet.length());

            if (!pb_decode(&stream, TestMessage_fields, &message)) {
                Serial.println("Failed to decode");
                return;
            }

            Serial.println(message.msg);

        });
    }

}

void sendMessage() {

    message.msg = 1.12f;

    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));

    bool status = pb_encode(&stream, TestMessage_fields, &message);



    if (!status) {
        Serial.println("Failed to encode");
        return;
    }

    /*
    Serial.print("Message Length: ");
    Serial.println(stream.bytes_written);

    Serial.print("Message: ");

    for(int i = 0; i<stream.bytes_written; i++){
        Serial.printf("%02X",buffer[i]);
    }
    Serial.println();
*/


    //send hello world to server
    udp.beginPacket(udpAddress, udpPort);
     udp.write(buffer,stream.bytes_written );
    udp.endPacket();

}

void loop() {

    Serial.println(i++);

    delay(7000);
    sendMessage();


}