#include <Arduino.h>

#define ESP8266_LED 5

void setup()
{
    pinMode(ESP8266_LED, OUTPUT);
}

void loop()
{
    digitalWrite(ESP8266_LED, true);
    delay(500);
    digitalWrite(ESP8266_LED, false);
    delay(500);
}
