#include "debugserial.h"
#include <Arduino.h>

void debugserial_init()
{
#ifdef DEBUGMODE
    Serial.begin(9600);
#endif
}

