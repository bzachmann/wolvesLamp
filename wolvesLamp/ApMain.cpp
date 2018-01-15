#include "ApMain.h"

#include <Arduino.h>

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
    streamerInfo.run();
}
