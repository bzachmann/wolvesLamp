#include "capacitivetouchbutton.h"

#define HOLD_THRESHOLD      (3000)
#define TAP_THRESHOLD       (50)

CapacitiveTouchButton::CapacitiveTouchButton(int8_t inputPin):
    inputPin(inputPin),
    hold(false),
    tap(false)
{

}

void CapacitiveTouchButton::init()
{
    pinMode(inputPin, INPUT);
}

void CapacitiveTouchButton::run()
{
    static bool prevInput = false;
    static bool ignoreFallingEdge = false;
    static uint32_t touchCounter = 0;
    static uint32_t timeOld, timeNew = millis();

    timeNew = millis();

    if(digitalRead(inputPin))
    {
        if(prevInput == false)
        {
            touchCounter = 0;
        }
        else
        {
            touchCounter += timeNew - timeOld;
            if(touchCounter > HOLD_THRESHOLD)
            {
                ignoreFallingEdge = true;
                hold = true;
            }
            else
            {
                ignoreFallingEdge = false;
                hold = false;
            }

        }
        tap = false;
        prevInput = true;
    }
    else
    {
        if((prevInput == true) && !ignoreFallingEdge && (touchCounter > TAP_THRESHOLD))
        {
            tap = true;
            touchCounter = 0;
        }
        else
        {
            tap = false;
            touchCounter = 0;
        }

        hold = false;
        ignoreFallingEdge = false;
        prevInput = false;
    }

    timeOld = timeNew;

}

bool CapacitiveTouchButton::getTap()
{
    return tap;
}

bool CapacitiveTouchButton::getHold()
{
    return hold;
}
