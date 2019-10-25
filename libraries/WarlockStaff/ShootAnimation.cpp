#include "ShootAnimation.h"

#include <avr/pgmspace.h>

void ShootAnimation::setup()
{
    start();
}

void ShootAnimation::start()
{
    phase = 0;
    startTime = micros();
    incrementer = 0;
}

void ShootAnimation::glowBase(uint16_t elapsed)
{
    const uint8_t GlowSize = 20;
    const uint16_t PhaseTime = 2000;
    const uint16_t FadeMs = 1000;
    const uint8_t FadeAmountPerFrame = min(255.0f / ((float)FadeMs / clock.getTargetMsPerFrame()), 255);

    if (elapsed >= PhaseTime)
    {
        return;
    }

    display.fadeToBlackBy(FadeAmountPerFrame);

    uint8_t dotChanceEased = ease8InOutCubic(static_cast<uint8_t>(min((elapsed / (float)FadeMs) * 255, 255)));
    if (random8() < dotChanceEased)
    {
        for (int16_t l = NumLedsPerStrip - GlowSize; l >= 0; l--)
        {
            uint8_t level = static_cast<uint8_t>(l / NumLedsPerStrip * 255.0f);

            for (uint8_t s = 0; s < NumStrips; s++)
            {
                if (random8() < 128)
                {
                    display.leds[s][l] = CRGB::Black;
                }
                else
                {
                    display.leds[s][l] = (random8() < 128) ? CRGB::LimeGreen : CRGB::White;
                    display.leds[s][l].nscale8(level);
                }
            }
        }
    }
}

void ShootAnimation::waves(uint16_t elapsed)
{
    const uint16_t PhaseStartTime = 1000;
    const uint16_t PhaseTime = 5000;
    const uint16_t FadeMs = 20;
    const uint8_t FadeAmountPerFrame = min(255.0f / ((float)FadeMs / clock.getTargetMsPerFrame()), 255);

    if (elapsed < PhaseStartTime || elapsed >= PhaseTime)
    {
        return;
    }

    display.fadeToBlackBy(FadeAmountPerFrame);

    uint16_t phaseElapsed = elapsed - PhaseStartTime;
    float percentDone = phaseElapsed / static_cast<float>(PhaseTime);
    uint8_t easedPercentDone = ease8InOutCubic(static_cast<uint8_t>(min(percentDone * 255, 255)));

    for (int16_t l = NumLedsPerStrip - 1; l >= 0; l--)
    {
        uint8_t angle = static_cast<uint8_t>(l / NumLedsPerStrip * 255.0f);
        uint8_t sin = sin8(angle + incrementer);
        uint8_t strip;

        if (sin < 85)
        {
            strip = 0;
        }
        else if (sin < 170)
        {
            strip = 1;
        }
        else
        {
            strip = 2;
        }

        display.leds[strip][l] = CRGB::LimeGreen;
        display.leds[strip][l] += (int(random8(10)) - 5);

        // Add some random crackle in places where there's no "shot"
        for (uint8_t s = 0; s < NumStrips; s++)
        {
            if (s == strip)
            {
                continue;
            }

            if (random8() < 25)
            {
                display.leds[s][l] = CRGB::White;
            }
        }
    }

    EVERY_N_MILLISECONDS(3)
    {
        incrementer++;
    }
}

void ShootAnimation::shots(uint16_t elapsed)
{
    const uint16_t PhaseStartTime = 4500;
    const uint16_t PhaseTime = 9500;
    const uint16_t FadeMs = 30;
    const uint8_t FadeAmountPerFrame = min(255.0f / ((float)FadeMs / clock.getTargetMsPerFrame()), 255);
    const uint8_t RingSizeOnTopAndBottom = 2;
    const uint8_t RingGradientChange = 20;
    const uint16_t MaxRingSpeed = 333;
    const uint16_t MinRingSpeed = 666;

    if (elapsed < PhaseStartTime || elapsed >= PhaseTime)
    {
        return;
    }

    display.fadeToBlackBy(FadeAmountPerFrame);

    uint16_t phaseElapsed = elapsed - PhaseStartTime;
    float percentDone = phaseElapsed / static_cast<float>(PhaseTime);
    uint16_t ringMoveSpeedMs = static_cast<uint16_t>((MinRingSpeed - MaxRingSpeed) * percentDone) + MaxRingSpeed;
    uint8_t numDesiredRings = static_cast<uint8_t>((MaxRings * percentDone) + 1);

    // Update rings
    ulong currentTime = micros();
    uint8_t ringsActive = 0;
    int8_t firstInactiveRing = -1;
    for (uint8_t r = 0; r < MaxRings; r++)
    {
        if (!ringActive[r])
        {
            if (firstInactiveRing == -1)
            {
                firstInactiveRing = r;
            }

            continue;
        }

        ringsActive++;

        // Draw ring
        float ringPercentDone = ((currentTime - ringFireTimes[r]) / 1000) / ringMoveSpeedMs;
        int16_t ringPosition = NumLedsPerStrip - (NumLedsPerStrip * ringPercentDone);

        bool active = false;
        for (int16_t led = ringPosition - RingSizeOnTopAndBottom; led <= ringPosition + RingSizeOnTopAndBottom; led++)
        {
            if (led < 0 || led >= NumLedsPerStrip)
            {
                continue;
            }

            uint8_t ringGradientChange = 255 - RingGradientChange * abs(led);

            for (int8_t strip = 0; strip < NumStrips; strip++)
            {
                active = true;
                display.leds[strip][led] = CRGB::LimeGreen;
                display.leds[strip][led].nscale8(ringGradientChange);
                display.leds[strip][led] += (int(random8(10)) - 5);
            }
        }

        // If all parts of the ring went off the end of the LED strips, mark it as inactive
        if (!active)
        {
            ringActive[r] = false;
        }
    }

    if (ringsActive < numDesiredRings)
    {
        ringActive[firstInactiveRing] = true;
        ringFireTimes[firstInactiveRing] = micros();
    }
}

void ShootAnimation::insanity(uint16_t elapsed)
{
    const uint16_t PhaseStartTime = 9500;
    const uint16_t PhaseTime = 12500;
    const uint16_t FadeMs = 1000;
    const uint8_t FadeAmountPerFrame = min(255.0f / ((float)FadeMs / clock.getTargetMsPerFrame()), 255);

    if (elapsed < PhaseStartTime || elapsed >= PhaseTime)
    {
        return;
    }

    display.fadeToBlackBy(FadeAmountPerFrame);
}

void ShootAnimation::sparkleAway(uint16_t elapsed)
{
    const uint16_t PhaseStartTime = 12500;
    const uint16_t PhaseTime = 15000;
    const uint16_t FadeMs = 1000;
    const uint8_t FadeAmountPerFrame = min(255.0f / ((float)FadeMs / clock.getTargetMsPerFrame()), 255);

    if (elapsed >= PhaseTime)
    {
        return;
    }

    display.fadeToBlackBy(FadeAmountPerFrame);
}

void ShootAnimation::loop()
{
    ulong elapsed = (micros() - startTime) / 1000;
    Serial.println(elapsed);

    glowBase(elapsed);
    waves(elapsed);
    shots(elapsed);
    insanity(elapsed);
    sparkleAway(elapsed);
}

ulong ShootAnimation::animationTime() const
{
    return 15000;
}
