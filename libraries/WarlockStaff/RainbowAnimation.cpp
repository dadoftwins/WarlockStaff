#include "RainbowAnimation.h"

void RainbowAnimation::setup()
{
    Serial.println("RainbowAnimation setup...");
}

void RainbowAnimation::loop()
{
    for (uint8_t strip = 0; strip < NumStrips; strip++)
    {
        fill_rainbow(display.leds[strip], NumLedsPerStrip, hue, 5);
    }

    EVERY_N_MILLISECONDS(1)
    {
        hue++;
    }
}
