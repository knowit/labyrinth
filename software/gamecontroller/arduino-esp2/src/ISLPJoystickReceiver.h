

#ifndef GAMECONTROLLER_ISLPJOYSTICKRECEIVER_H
#define GAMECONTROLLER_ISLPJOYSTICKRECEIVER_H

#include <WiFiUdp.h>
#include <WiFi.h>
#include "AsyncUDP.h"

#include "pb_common.h"
#include "pb.h"
#include "pb_encode.h"
#include "pb_decode.h"
#include "./pb/GameMessage.pb.h"


class ISLPJoystickReceiver {

private:


public:

    AsyncUDP asyncUdp;

    ISLPJoystickReceiver();

    int setup(int port);

    double x;
    double y;

};


#endif //GAMECONTROLLER_JOYSTICK_H
