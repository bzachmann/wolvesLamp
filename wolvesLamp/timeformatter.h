#ifndef TIMEFORMATTER_H
#define TIMEFORMATTER_H

#include <Arduino.h>

class TimeFormatter
{
public:
    TimeFormatter() :
        seconds(0) {}

    void setSeconds(uint32_t sec);
    uint16_t getHours();
    uint16_t getMinutes();
    String getFormattedTime();

private:
    uint32_t seconds;
};

#endif // TIMEFORMATTER_H
