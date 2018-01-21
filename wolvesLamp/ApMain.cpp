#include "ApMain.h"

#include <Arduino.h>
#include "debugserial.h"
#include "timeformatter.h"

//#define TWITCH_NAME     ("wolvesatmydoor")
//#define TWITCH_CHANNEL_ID       ("42781716")
//#define TWITCH_NAME             ("guude")
//#define TWITCH_CHANNEL_ID       ("20730412")
#define TWITCH_NAME             ("anderzel")
#define TWITCH_CHANNEL_ID       ("20730412")

//#define TWITCH_TARGET_UPTIME    (6.0f) //6 hours
#define TWITCH_TARGET_UPTIME    (0.05f) //3 minutes


ApMain ApMain::inst;

ApMain::ApMain() :
    streamerInfo(TWITCH_NAME, TWITCH_CHANNEL_ID, TWITCH_TARGET_UPTIME),
    streamerDisplay(),
    ledStrip()
{

}

void ApMain::init()
{
    streamerInfo.init();
    streamerDisplay.init();
    ledStrip.init();
}

void ApMain::run()
{
    static uint16_t timeOld, timeNew = millis();
    static uint16_t timeout = 0;
    timeNew = millis();

    if(timeout < 100)
    {
        timeout += timeNew - timeOld;
    }
    else
    {
        timeout = 0;
        if(streamerInfo.isLive())
        {
            static uint32_t upTime;
            upTime = streamerInfo.getUpTimeSeconds();
            debugPrint("Uptime: ");
            debugPrint(upTime);
            debugPrint(" seconds\n");
        }
    }

    streamerInfo.run();
    streamerDisplay.setStreaming(streamerInfo.isLive());
    streamerDisplay.setHosting(streamerInfo.isHosting());
    streamerDisplay.setGameName(streamerInfo.getGameName());
    streamerDisplay.setHostName(streamerInfo.getHostName());
    streamerDisplay.setHostGameName(streamerInfo.getHostGameName());

    TimeFormatter timeformatter;
    timeformatter.setSeconds(streamerInfo.getUpTimeSeconds());
    streamerDisplay.setUpTime(timeformatter.getFormattedTime());
    streamerDisplay.run();

    ledStrip.setState(streamerInfo.isLive());
    ledStrip.setProgress(streamerInfo.getProgress());
    ledStrip.run();


    timeOld = timeNew;
}
