#include "twitchinfodisplay.h"
#include "twitchstreamerinfo.h"

#define OLED_RESET 2

MarqueeString::Config_t const TwitchInfoDisplay::gameNameConfig =
{
    .screenLength = 21,
    .scrollDelayMs = 5000,
    .scrollSpeed = 6
};

MarqueeString::Config_t const TwitchInfoDisplay::hostNameConfig =
{
    .screenLength = 7,
    .scrollDelayMs = 5000,
    .scrollSpeed = 6
};

MarqueeString::Config_t const TwitchInfoDisplay::hostGameNameConfig =
{
    .screenLength = 21,
    .scrollDelayMs = 5000,
    .scrollSpeed = 6
};

TwitchInfoDisplay::TwitchInfoDisplay():
    streaming(0),
    hosting(0),
    errorCode(TwitchStreamerInfo::ERROR_NO_ERROR),
    upTime(0),
    gameName(gameNameConfig),
    hostName(hostNameConfig),
    hostGameName(hostGameNameConfig),
    display(OLED_RESET)
{

}

void TwitchInfoDisplay::init()
{
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.setCursor(0,0);
    display.setTextColor(WHITE);
}

void TwitchInfoDisplay::run()
{
    if(errorCode != TwitchStreamerInfo::ERROR_NO_ERROR)
    {
        displayError();
    }
    else if(hosting)
    {
        displayHosting();
        hostName.run();
        hostGameName.run();
    }
    else if(streaming)
    {
        displayOnline();
        gameName.run();
    }
    else
    {
        displayOffline();
    }

}

void TwitchInfoDisplay::setStreaming(bool status)
{
    streaming = status;
}

void TwitchInfoDisplay::setHosting(bool status)
{
    hosting = status;
}

void TwitchInfoDisplay::setErrorCode(uint8_t error)
{
    errorCode = error;
}

void TwitchInfoDisplay::setUpTime(String uptime)
{
    upTime = uptime;
}

void TwitchInfoDisplay::setGameName(String name)
{
    gameName.setString(name);
}

void TwitchInfoDisplay::setHostName(String name)
{
    hostName.setString(name);
}

void TwitchInfoDisplay::setHostGameName(String name)
{
    hostGameName.setString(name);
}

void TwitchInfoDisplay::displayOffline()
{
    display.clearDisplay();
    display.setCursor(0,0);
    display.setTextSize(2);
    display.println("Offline");
    display.display();
}

void TwitchInfoDisplay::displayOnline()
{
    display.clearDisplay();
    display.setCursor(0,0);
    display.setTextSize(2);
    display.println("LIVE");
    display.setTextSize(1);
    display.println();
    display.setTextSize(3);
    display.println(upTime);
    display.setTextSize(1);
    display.println();
    display.setTextSize(1);
    display.println(gameName.getSubString());
    display.display();

}

void TwitchInfoDisplay::displayHosting()
{
    display.clearDisplay();
    display.setCursor(0,0);
    display.setTextSize(2);
    display.println("Hosting");
    display.setTextSize(1);
    display.println();
    display.setTextSize(3);
    display.println(hostName.getSubString());
    display.setTextSize(1);
    display.println();
    display.setTextSize(1);
    display.println(hostGameName.getSubString());
    display.display();

}

void TwitchInfoDisplay::displayError()
{
    display.clearDisplay();
    display.setCursor(0,0);
    display.setTextSize(2);
    display.println("ERROR");
    display.println(errorCode);
    display.display();
}
