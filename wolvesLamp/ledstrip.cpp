#include "ledstrip.h"
#include "debugserial.h"

void LEDStrip::init()
{
    leds.begin();
}

void LEDStrip::run()
{
    if((state != 0) && !error)
    {
        leds.setBrightness(brightness);
    }
    else
    {
        leds.setBrightness(0);
    }

    uint8_t r, g, b = 0;
    settings.getRGB(r, g, b);
    for(uint8_t i = 0; i < NUM_LEDS; i++)
    {
        leds.setPixelColor(i, r, g, b);
    }

    leds.show();
}

void LEDStrip::toggleBrightness()
{
    brightnessLevel++;
    if(brightnessLevel == BRIGHTNESS_LEVELS)
    {
        brightnessLevel = 0;
    }


    brightness = (uint8_t)(255/BRIGHTNESS_LEVELS) * (brightnessLevel + 1);
}

void LEDStrip::setBrightness(uint8_t value)
{
    brightness = value;
}

void LEDStrip::setState(bool value)
{
    static bool prevState = false;
    if(value & !prevState)
    {
        brightness = DEFAULT_BRIGHTNESS;
    }
    state = value;
    prevState = state;
}

void LEDStrip::setProgress(uint8_t value)
{
    settings.setProgress(value);
}

void LEDStrip::setError(bool value)
{
    error = value;
}