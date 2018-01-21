/*
 * LEDSetting.cpp
 *
 *  Created on: Aug 10, 2017
 *      Author: Boston
 */

#include "LEDSetting.h"

void LEDSetting::setState(bool const &value)
{
	state = value;
}

void LEDSetting::setProgress(uint8_t const &value)
{
    progress = value;
}

void LEDSetting::getRGB(uint8_t &r, uint8_t &g, uint8_t &b)
{
    if(state != false)
    {
        r = progress;
        g = 0;
        b = 255 - progress;
    }
    else
    {
        r = 0;
        g = 0;
        b = 0;
    }
}
