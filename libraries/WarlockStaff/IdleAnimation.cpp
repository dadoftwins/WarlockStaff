#include "IdleAnimation.h"

void IdleAnimation::setup(uint16_t msToSweep, uint16_t msToFade, CRGB color)
{
    Serial.println("IdleAnimation setup...");
    this->color = color;
    this->msToSweep = msToSweep;

    direction = true;

    startTime = micros();
    
    fadeAmountPerFrame = 255 / (msToFade / clock.getTargetMsPerFrame());
    Serial.print("IdleAnimation fadeAmountPerFrame: ");
    Serial.println(fadeAmountPerFrame);
}

void IdleAnimation::loop()
{
    display.fadeToBlackBy(fadeAmountPerFrame);

    ulong timeSinceStart = (micros() - startTime) / 1000.0f;
    float percentDone = fmin(timeSinceStart / (float)msToSweep, 1.0f);
    uint8_t easedAmount = static_cast<uint8_t>(0xff * percentDone);
    uint8_t position = ease8InOutQuad(easedAmount);
    position = (position / 255.0f) * (NumLedsPerStrip - 1);

    uint8_t led = direction ? position : (NumLedsPerStrip - 1) - position;

    CRGB adjustedColor = brightness < 255 ? color.subtractFromRGB(255 - brightness) : color;
    for (uint8_t strip = 0; strip < NumStrips; strip++)
    {
        display.leds[strip][led] = adjustedColor;
    }

    if (percentDone == 1.0f)
    {
        direction = !direction;
        startTime = micros();
    }
}
