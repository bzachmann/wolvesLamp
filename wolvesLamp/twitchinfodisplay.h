#ifndef TWITCHINFODISPLAY_H
#define TWITCHINFODISPLAY_H

#include "marqueestring.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>




class TwitchInfoDisplay
{
public:
    TwitchInfoDisplay();

    void init();
    void run();

    void setStreaming(bool status);
    void setHosting(bool status);
    void setErrorCode(uint8_t error);
    void setUpTime(String uptime);
    void setGameName(String name);
    void setHostName(String name);
    void setHostGameName(String name);

private:
    void displayOffline();
    void displayOnline();
    void displayHosting();
    void displayError();

private:
    static MarqueeString::Config_t const gameNameConfig;
    static MarqueeString::Config_t const hostNameConfig;
    static MarqueeString::Config_t const hostGameNameConfig;

    bool streaming;
    bool hosting;
    uint8_t errorCode;

    String upTime;
    MarqueeString gameName;
    MarqueeString hostName;
    MarqueeString hostGameName;

    Adafruit_SSD1306 display;

};

#endif // TWITCHINFODISPLAY_H
