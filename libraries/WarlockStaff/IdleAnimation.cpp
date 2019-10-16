#include "IdleAnimation.h"

void IdleAnimation::setup(uint16_t msToSweep = 1000, uint16_t msToFade = 100, CRGB color = CRGB::White)
{
    Serial.println("IdleAnimation setup...");
    this->color = color;

    direction = true;

    easing = Easing(ease_mode::EASE_IN_OUT_CUBIC, msToSweep);
    easing.SetSetpoint(NumLedsPerStrip - 1);
    
    fadeAmountPerFrame = 255 / (msToFade / clock.getTargetMsPerFrame());
    Serial.print("IdleAnimation fadeAmountPerFrame: ");
    Serial.println(fadeAmountPerFrame);
}

void IdleAnimation::loop()
{
    display.fadeToBlackBy(fadeAmountPerFrame);

    float easeVal = easing.GetValue();
    uint8_t position = static_cast<uint8_t>(easing.GetValue());

    uint8_t led = direction ? position : (NumLedsPerStrip - 1) - position;

    CRGB adjustedColor = brightness < 255 ? color.subtractFromRGB(255 - brightness) : color;
    for (uint8_t strip = 0; strip < NumStrips; strip++)
    {
        display.leds[strip][led] = adjustedColor;
    }

    if (!easing.IsActive())
    {
        direction = !direction;
        easing.Reset();
        easing.SetSetpoint(NumLedsPerStrip - 1);        
    }
}
