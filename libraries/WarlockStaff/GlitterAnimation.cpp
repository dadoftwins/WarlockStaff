#include "GlitterAnimation.h"

void GlitterAnimation::setup(
    uint16_t popInMs,
    uint16_t fadeMs,
    uint8_t maxSparkles,
    uint8_t chanceOfSparkle,
    CRGB sparkleColor,
    bool randomColor)
{
    Serial.println("GlitterAnimation setup...");

    this->popInMs = popInMs;
    this->fadeMs = fadeMs;
    fadeAmountPerFrame = 255 / (fadeMs / clock.getTargetMsPerFrame());

    this->maxSparkles = maxSparkles;
    this->chanceOfSparkle = chanceOfSparkle;
    this->color = color;
    this->randomColor = randomColor;
}

void GlitterAnimation::loop()
{
    display.fadeToBlackBy(fadeAmountPerFrame);

    EVERY_N_MILLISECONDS(popInMs)
    {
        if (random8() < chanceOfSparkle)
        {
            uint8_t randomStrip = random8(NumStrips);
            uint8_t randomLed = random8(NumLedsPerStrip);
            if (!randomColor)
            {
                display.leds[randomStrip][randomLed] = color.subtractFromRGB(255 - brightness);
            }
            else
            {
                display.leds[randomStrip][randomLed] = CHSV(random8(255), 200, brightness);
            }
        }
    }
}
