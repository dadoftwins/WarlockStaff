#include "PulseFireAnimation.h"

void PulseFireAnimation::setup()
{
    reverseDirection = true;
    pallette = HeatColors_p;
    //cooling = 55;
    //sparking = 120;
    cooling = 60;
    sparking = 120;
}

void PulseFireAnimation::fireOnStrip(uint8_t strip)
{
    random16_add_entropy(random());

    // Array of temperature readings at each simulation cell
    static uint8_t heat[NumStrips][NumLedsPerStrip];

    // Step 1.  Cool down every cell a little
    for (uint8_t i = 0; i < NumLedsPerStrip; i++)
    {
        heat[strip][i] = qsub8(heat[strip][i], random8(0, ((cooling * 10) / NumLedsPerStrip) + 2));
    }

    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for (uint8_t k = NumLedsPerStrip - 1; k >= 2; k--)
    {
        heat[strip][k] = (heat[strip][k - 1] + heat[strip][k - 2] + heat[strip][k - 2] ) / 3;
    }

    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if (random8() < sparking)
    {
        int y = random8(7);
        heat[strip][y] = qadd8(heat[strip][y], random8(160,255));
    }

    // Step 4.  Map from heat cells to LED colors
    for (int j = 0; j < NumLedsPerStrip; j++)
    {
        // Scale the heat value from 0-255 down to 0-240
        // for best results with color palettes.
        byte colorindex = scale8(heat[strip][j], 240);
        CRGB color = ColorFromPalette(pallette, colorindex);
        uint16_t pixelnumber;
        if (reverseDirection)
        {
            pixelnumber = (NumLedsPerStrip - 1) - j;
        }
        else
        {
            pixelnumber = j;
        }
        
        display.leds[strip][pixelnumber] = color;
    }
}

void PulseFireAnimation::loop()
{
    EVERY_N_MILLISECONDS(16)
    {
        for (uint8_t s = 0; s < NumStrips; s++)
        {
            fireOnStrip(s);
        }
    }
}
