#include "marqueestring.h"
#include "debugserial.h"

void MarqueeString::init()
{
    reset();
}

void MarqueeString::run()
{
    if(fullString.length() > configuration.screenLength)
    {
        updateSubstring();
    }
}

void MarqueeString::reset()
{
    stringIndex = 0;
    shiftString = fullString;

    for(uint16_t i = 0; i < configuration.screenLength; i++)
    {
        shiftString.concat(' ');
    }
}

void MarqueeString::setString(String string)
{
    if(fullString != string)
    {
        fullString = string;
        reset();
    }
}

String MarqueeString::getSubString()
{
    return shiftString.substring(0, configuration.screenLength - 1);
}

void MarqueeString::updateSubstring()
{
    static uint32_t oldTime, newTime = millis();

    static MarqueeState_t marqueeState = STATE_DELAY;

    newTime = millis();
    uint32_t elapsedTime = newTime - oldTime;

    switch(marqueeState)
    {

    case STATE_DELAY:
    {
        static uint32_t delayTime = 0;
        delayTime += elapsedTime;
        if(delayTime > configuration.scrollDelayMs)
        {
            marqueeState = STATE_SCROLL;
            delayTime = 0;
        }
        break;
    }

    case STATE_SCROLL:
    {
        static float elapsedLetters = 0;
        elapsedLetters += elapsedTime / 1000.0 * configuration.scrollSpeed;
        uint8_t lettersToStep = (uint8_t)elapsedLetters;
        for(uint8_t i = 0; i < lettersToStep; i++)
        {
            stepShiftString();
            elapsedLetters -= 1.0;
            if(stringIndex == 0)
            {
                elapsedLetters = 0;
                marqueeState = STATE_DELAY;
            }
        }
        break;
    }
    default:
        break;
    }
    oldTime = newTime;
}

void MarqueeString::stepShiftString()
{
    char shiftChar = shiftString.charAt(0);
    shiftString = shiftString.substring(1, shiftString.length());
    shiftString.concat(shiftChar);

    debugPrint(shiftString);
    debugPrint("\n");

    stringIndex++;
    if(stringIndex == shiftString.length())
    {
        stringIndex = 0;
    }
}
