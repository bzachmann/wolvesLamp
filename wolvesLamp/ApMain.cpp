#include "ApMain.h"

#include <Arduino.h>

//#define TWITCH_NAME     "wolvesatmydoor"
#define TWITCH_NAME     "anderzel"

ApMain ApMain::inst;
ApMain::ApMain() :
    streamerInfo(TWITCH_NAME)
{

}

void ApMain::init()
{
}

void ApMain::run()
{
    streamerInfo.run();
}
