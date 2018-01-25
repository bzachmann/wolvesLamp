#include "ApMain.h"

#include <Arduino.h>
#include "debugserial.h"
#include "timeformatter.h"

#define TWITCH_NAME     ("wolvesatmydoor")
#define TWITCH_CHANNEL_ID       ("41721716")
//#define TWITCH_NAME             ("guude")
//#define TWITCH_CHANNEL_ID       ("20730412")
//#define TWITCH_NAME             ("anderzel")
//#define TWITCH_CHANNEL_ID       ("20730412")

#define TWITCH_TARGET_UPTIME    (6.0f) //6 hours
#define PING_INTERVAL_S         (300000) // 5 minutes
//#define PING_INTERVAL_S         (5000) // 5 minutes
#define ERROR_TOLERANCE         (3)


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
    static uint8_t errorCounter = 0;
    static uint32_t timeOld, timeNew = millis();
    timeNew = millis();

    streamerInfo.run();


    if(streamerInfo.isDone())
    {
        static uint32_t timeout = 0;
        errorCounter = 0;
        timeout += timeNew - timeOld;

        if(timeout > PING_INTERVAL_S)
        {
            streamerInfo.reset();
            timeout = 0;
        }
    }
    else if(streamerInfo.isInError())
    {
        if(errorCounter > ERROR_TOLERANCE)
        {
            streamerDisplay.setErrorCode(streamerInfo.getError());
            ledStrip.setError(true);
        }
        else
        {
            errorCounter++;
            streamerInfo.reset();
        }

    }

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
