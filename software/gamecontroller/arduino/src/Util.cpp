

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

float read2byteFloat() {
    int msb = Serial.read();
    if (msb < 64) return -999;
    int lsb = Serial.read();
    if (lsb < 64) return -999;
    return (msb - 64) * 192 + (lsb - 64);
}

#endif