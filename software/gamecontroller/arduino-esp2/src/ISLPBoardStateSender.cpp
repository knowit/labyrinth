
#include "ISLPBoardStateSender.h"

ISLPBoardStateSender::ISLPBoardStateSender() {
}

int ISLPBoardStateSender::setup() {
    udp.begin(boardStateUDPPort);
    return 0;
}

void ISLPBoardStateSender::update() {
    // Serial.println("sendUDPMessage");

    BoardState boardState = BoardState_init_zero;
    boardState.has_orientation=true;
    boardState.orientation.x =x;
    boardState.orientation.y = y;

    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));

    bool status = pb_encode(&stream, BoardState_fields, &boardState);

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


    udp.beginPacket(udpAddress, boardStateUDPPort);
    udp.write(buffer,stream.bytes_written );
    udp.endPacket();

}

