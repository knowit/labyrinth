
#ifndef GAMECONTROLLER_UTIL_H
#define GAMECONTROLLER_UTIL_H

#include <Arduino.h>

byte msb(int numberToSend);
byte lsb(int numberToSend);
void write2byteFloat(int pseudoValue);
float read2byteFloat();

#endif //GAMECONTROLLER_UTIL_H
