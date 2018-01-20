#ifndef APMAIN_H
#define APMAIN_H

#include "twitchstreamerinfo.h"

#include "marqueestring.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class ApMain {

public:
    ApMain();
    void init();
    void run();

public:
    static ApMain inst;

private:
    static MarqueeString::Config_t const marqueeConfig;

    TwitchStreamerInfo streamerInfo;
    MarqueeString marqueeString;

    Adafruit_SSD1306 display;

};

#endif //APMAIN_H
