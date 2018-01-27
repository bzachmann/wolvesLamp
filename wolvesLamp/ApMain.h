#ifndef APMAIN_H
#define APMAIN_H

#include "twitchstreamerinfo.h"
#include "twitchinfodisplay.h"
#include "ledstrip.h"
#include "capacitivetouchbutton.h"

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
    CapacitiveTouchButton button;
};

#endif //APMAIN_H
