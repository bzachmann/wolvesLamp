#include "timeformatter.h"

void TimeFormatter::setSeconds(uint32_t sec)
{
    seconds = sec;
}

uint16_t TimeFormatter::getHours()
{
    return seconds / 3600;
}

uint16_t TimeFormatter::getMinutes()
{
    uint16_t remainderSeconds = seconds % 3600;
    return remainderSeconds / 60;
}

String TimeFormatter::getFormattedTime()
{
    String retString = "";
    uint16_t hours = getHours();
    if(hours > 99)
    {
        hours = 99;
    }

    uint16_t minutes = getMinutes();

    char tempChar = (hours / 10) + '0';
    retString.concat(tempChar);
    tempChar = (hours % 10) + '0';
    retString.concat(tempChar);
    retString.concat(':');
    tempChar = (minutes / 10) + '0';
    retString.concat(tempChar);
    tempChar = (minutes % 10) + '0';
    retString.concat(tempChar);

    return retString;
}
