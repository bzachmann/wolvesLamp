#ifndef APMAIN_H
#define APMAIN_H

#include "twitchstreamerinfo.h"
#include "twitchinfodisplay.h"
#include "ledstrip.h"

class ApMain {

public:
    ApMain();
    void init();
    void run();

public:
    static ApMain inst;

private:
    TwitchStreamerInfo streamerInfo;
    TwitchInfoDisplay streamerDisplay;
    LEDStrip ledStrip;
};

#endif //APMAIN_H
