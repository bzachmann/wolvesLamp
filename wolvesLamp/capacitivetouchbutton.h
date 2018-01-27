#ifndef CAPACITIVETOUCHBUTTON_H
#define CAPACITIVETOUCHBUTTON_H

#include <Arduino.h>

class CapacitiveTouchButton
{
public:
    CapacitiveTouchButton(int8_t inputPin);
    void init();
    void run();

    bool getTap();
    bool getHold();

private:
    int8_t const inputPin;
    bool hold;
    bool tap;
};

#endif // CAPACITIVETOUCHBUTTON_H
