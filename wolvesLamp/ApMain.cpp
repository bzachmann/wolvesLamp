#include "ApMain.h"

#include <Arduino.h>
#include "debugserial.h"

//#define TWITCH_NAME     ("wolvesatmydoor")
//#define TWITCH_CHANNEL_ID       ("42781716")
#define TWITCH_NAME             ("guude")
#define TWITCH_CHANNEL_ID       ("20730412")


#define OLED_RESET 2

ApMain ApMain::inst;

MarqueeString::Config_t const ApMain::marqueeConfig =
{
    .screenLength = 21,
    .scrollDelayMs = 10000,
    .scrollSpeed = 6
};


ApMain::ApMain() :
    streamerInfo(TWITCH_NAME, TWITCH_CHANNEL_ID),
    marqueeString(marqueeConfig),
    display(OLED_RESET)
{

}

void ApMain::init()
{
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
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

    marqueeString.setString("Playing: PLAYERUNKNOWN'S BATTLEGROUNDS");
    marqueeString.run();

    display.setTextColor(WHITE);
    display.clearDisplay();
    display.setCursor(0,0);
    display.println(marqueeString.getSubString());
    display.display();


    timeOld = timeNew;
}
