#ifndef LEDSTRIP_H
#define LEDSTRIP_H

#include <Arduino.h>
#include "ledsetting.h"
#include <Adafruit_NeoPixel.h>

#define NUM_LEDS                        (8)
#define LED_PIN                         (14)
#define BRIGHTNESS_LEVELS               (5)
#define DEFAULT_BRIGHTNESS              (128)

class LEDStrip
{
public:
    LEDStrip():
        brightness(0),
        brightnessLevel(0),
        state(0),
        settings(),
        leds(NUM_LEDS, LED_PIN),
        error(false){}
    void init();
    void run();

    void toggleBrightness();
    void setBrightness(uint8_t value);
    void setState(bool value);
    void setProgress(uint8_t value);
    void setError(bool value);

private:
    uint8_t brightness;
    uint8_t brightnessLevel;
    uint8_t state;
    LEDSetting settings;

    Adafruit_NeoPixel leds;

    bool error;

};

#endif // LEDSTRIP_H
