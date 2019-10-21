#include "BeatStripsAnimation.h"

void BeatStripsAnimation::setup()
{
    Serial.println("BeatStripsAnimation setup...");
    
    bpm = 62;
    hue = 0;
}

void BeatStripsAnimation::loop()
{
    // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
    CRGBPalette16 palette = PartyColors_p;
    uint8_t beat = beatsin8(bpm, 64, 255);
    for (uint8_t s = 0; s < NumStrips; s++)
    {
        for (int i = 0; i < NumLedsPerStrip; i++)
        {
            display.leds[s][i] = ColorFromPalette(palette, hue + (i * 2), beat - hue + (i * 10));
        }
    }

    EVERY_N_MILLISECONDS(1)
    {
        hue++;
    }

    EVERY_N_MILLISECONDS(500)
    {
        bpm++;
    }
}
