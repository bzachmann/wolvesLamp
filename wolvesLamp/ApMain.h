#ifndef APMAIN_H
#define APMAIN_H

#include "twitchstreamerinfo.h"

class ApMain {

public:
    ApMain();
    void init();
    void run();

public:
    static ApMain inst;

private:

    TwitchStreamerInfo streamerInfo;

};

#endif //APMAIN_H
