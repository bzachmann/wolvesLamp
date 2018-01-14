#include <Arduino.h>
#include "apmain.h"
#include "debugserial.h"

void setup()
{
    debugserial_init();
    ApMain::inst.init();
}

void loop()
{
    ApMain::inst.run();
}
