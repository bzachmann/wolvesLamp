#ifndef MARQUEESTRING_H
#define MARQUEESTRING_H

#include <Arduino.h>

class MarqueeString
{
private:
    enum MarqueeState_t
    {
        STATE_DELAY = 0,
        STATE_SCROLL,
    };


public:
    typedef struct _Config_t
    {
        uint16_t screenLength; //in characters
        uint16_t scrollDelayMs;  //amount of time before starting scroll
        uint16_t scrollSpeed; //characters per second

    }Config_t;

public:
    MarqueeString(Config_t config):
        configuration(config),
        fullString(""),
        shiftString(""),
        stringIndex(0){}

    void init();
    void run();
    void reset();
    void setString(String string);
    String getSubString();

private:
    void updateSubstring();
    void stepShiftString();

private:
    Config_t const configuration;
    String fullString;
    String shiftString;
    uint16_t stringIndex;
};

#endif // MARQUEESTRING_H
