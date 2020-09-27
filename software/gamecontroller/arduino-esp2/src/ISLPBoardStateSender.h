


#ifndef GAMECONTROLLER_ISLPBOARDSTATESENDER_H
#define GAMECONTROLLER_ISLPBOARDSTATESENDER_H

#include <Arduino.h>
#include "pb_common.h"
#include "pb.h"
#include "pb_encode.h"
#include "pb_decode.h"
#include "./pb/GameMessage.pb.h"
#include <WiFiUdp.h>

class ISLPBoardStateSender {

private:
    int boardStateUDPPort = 4049;
    char *udpAddress = "192.168.10.199";
    WiFiUDP udp;
    uint8_t buffer[512];


public:

    ISLPBoardStateSender();

    void update();
    int setup();

    double x;
    double y;

};


#endif
