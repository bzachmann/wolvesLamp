#include "ApMain.h"

#include <Arduino.h>
#include "debugserial.h"

//#define TWITCH_NAME     ("wolvesatmydoor")
//#define TWITCH_ID       ("42781716")
#define TWITCH_NAME             ("guude")
#define TWITCH_CHANNEL_ID       ("20730412")


ApMain ApMain::inst;
ApMain::ApMain() :
    streamerInfo(TWITCH_NAME, TWITCH_CHANNEL_ID)
{

}

void ApMain::init()
{
}

void ApMain::run()
{
    static uint16_t timeOld, timeNew = millis();
    static uint16_t timeout = 0;
    timeNew = millis();

    if(timeout < 5000)
    {
        timeout += timeNew - timeOld;
    }
    else
    {
        timeout = 0;
        if(streamerInfo.isLive())
        {
            debugPrint("Uptime: ");
            debugPrint(streamerInfo.getUpTimeSeconds());
            debugPrint(" seconds\n");
        }
    }

    streamerInfo.run();
    timeOld = timeNew;
}
