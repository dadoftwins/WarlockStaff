#include "JuggleDotsAnimation.h"

void JuggleDotsAnimation::setup()
{
    Serial.println("JuggleDotsAnimation setup...");
}

void JuggleDotsAnimation::loop()
{
    // colored dots, weaving in and out of sync with each other
    uint8_t dothue = 0;
    for (uint8_t s = 0; s < NumStrips; s++)
    {
        fadeToBlackBy(display.leds[s], NumLedsPerStrip, 20);
        for (int i = 0; i < 8; i++)
        {
            display.leds[s][beatsin16( i+7, 0, NumLedsPerStrip-1 )] |= CHSV(dothue, 200, 255);
            dothue += 32;
        }
    }
}
