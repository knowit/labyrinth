

#ifndef GAMECONTROLLER_UTIL_CPP
#define GAMECONTROLLER_UTIL_CPP

#include <Util.h>

byte msb(int numberToSend) {
    return 64 + numberToSend / 192;
}

byte lsb(int numberToSend) {
    return 64 + numberToSend % 192;
}

void write2byteFloat(int pseudoValue) {
    Serial.write(msb(pseudoValue));
    Serial.write(lsb(pseudoValue));
}

#endif