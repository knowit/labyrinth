

#include "ISLPJoystickReceiver.h"

ISLPJoystickReceiver::ISLPJoystickReceiver() {
}

JoystickState message = JoystickState_init_zero;

int ISLPJoystickReceiver::setup(int port) {

    if (asyncUdp.listen(port)) {
        Serial.print("UDP Listening on IP: ");
        Serial.println(WiFi.localIP());
        asyncUdp.onPacket([this](AsyncUDPPacket packet) {

            /*
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

             */

            //Serial.println();
            //String myString = (const char *) packet.data();

            pb_istream_t stream = pb_istream_from_buffer(packet.data(), packet.length());


            if (!pb_decode(&stream, JoystickState_fields, &message)) {
                Serial.println("Failed to decode");
                return;
            }

            Serial.print(".");

            x = message.orientation.x;
            y = 0 - message.orientation.y;


        });
    }

    return 0;
}


