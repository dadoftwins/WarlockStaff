#include "RainbowAnimation.h"

void RainbowAnimation::setup()
{
    Serial.println("RainbowAnimation setup...");
    brightness = 0;
}

void fill_pallette(struct CRGB* pFirstLED, int numToFill, uint8_t initialhue, uint8_t deltahue, uint8_t brightness, const CRGBPalette16& pallette)
{
    for (int i = 0; i < numToFill; i++)
    {
        pFirstLED[i] = ColorFromPalette(pallette, initialhue + i + deltahue, brightness, LINEARBLEND);
    }
}

void RainbowAnimation::loop()
{
    brightness += 6;
    if (brightness >= 252)
    {
        direction = !direction;
        brightness = 0;
    }
    
    for (uint8_t strip = 0; strip < NumStrips; strip++)
    {
        if (ocean)
        {
            fill_pallette(display.leds[strip], NumLedsPerStrip, hue, 7, direction ? brightness : 0xff - brightness, OceanColors_p);
        }
        else
        {
            fill_rainbow(display.leds[strip], NumLedsPerStrip, hue, 7);
        }
    }

    EVERY_N_MILLISECONDS(2)
    {
        hue += 1;
    }
}
