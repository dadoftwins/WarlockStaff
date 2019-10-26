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
    const uint8_t GlowSize = 60;
    const uint16_t PhaseTime = 15000;
    const uint16_t FadeInTime = 5000;
    const uint16_t FadeMs = 200;
    const uint8_t MaxSparkleChance = 10;
    const uint8_t FadeAmountPerFrame = min(255.0f / ((float)FadeMs / clock.getTargetMsPerFrame()), 255);

    if (elapsed >= PhaseTime)
    {
        return;
    }

    display.fadeToBlackBy(FadeAmountPerFrame);

    float fadePercent = min(elapsed / static_cast<float>(FadeInTime), 1.0f);
    uint8_t sparkleChance = static_cast<uint8_t>(fadePercent * MaxSparkleChance);

    for (uint8_t l = NumLedsPerStrip - 1, i = 0; l >= NumLedsPerStrip - GlowSize; l--, i++)
    {
        uint8_t level = static_cast<uint8_t>((GlowSize - i) / (float)GlowSize * 255.0f);

        for (uint8_t s = 0; s < NumStrips; s++)
        {
            if (random8() < sparkleChance)
            {
                CRGB color = ColorFromPalette(CloudColors_p, random());
                display.leds[s][l] = color;
                display.leds[s][l].nscale8(level);
            }
        }
    }
}

void ShootAnimation::shots(uint16_t elapsed)
{
    const uint16_t PhaseStartTime = 5000;
    const uint16_t PhaseTime = 15000;
    const uint16_t FadeMs = 200;
    const uint8_t FadeAmountPerFrame = min(255.0f / ((float)FadeMs / clock.getTargetMsPerFrame()), 255);
    const uint8_t RingSizeOnTopAndBottom = 3;
    const uint8_t RingGradientChange = 15;
    const uint16_t MaxRingSpeed = 100;
    const uint16_t MinRingSpeed = 750;

    if (elapsed < PhaseStartTime || elapsed >= PhaseTime)
    {
        return;
    }

    display.fadeToBlackBy(FadeAmountPerFrame);

    uint16_t phaseElapsed = elapsed - PhaseStartTime;
    float percentDone = phaseElapsed / static_cast<float>(PhaseTime);
    uint16_t ringMoveSpeedMs = MinRingSpeed - static_cast<uint16_t>((MinRingSpeed - MaxRingSpeed) * percentDone);
    uint8_t numDesiredRings = static_cast<uint8_t>((MaxRings * percentDone) + 1);

    // Update rings
    ulong currentTime = micros();
    uint8_t ringsActive = 0;
    int8_t firstInactiveRing = -1;
    float closestRing = 1.0f;
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
        float ringPercentDone = ((currentTime - ringFireTimes[r]) / 1000.0f) / ringMoveSpeedMs;
        if (ringPercentDone < closestRing)
        {
            closestRing = ringPercentDone;
        }
        int16_t ringPosition = NumLedsPerStrip - (NumLedsPerStrip * ringPercentDone);

        bool active = false;
        int16_t ringGradient = -RingGradientChange * RingSizeOnTopAndBottom;
        for (int16_t led = ringPosition - RingSizeOnTopAndBottom; led <= ringPosition + RingSizeOnTopAndBottom; led++, ringGradient += RingGradientChange)
        {
            if (led < 0 || led >= NumLedsPerStrip)
            {
                continue;
            }

            uint8_t ringGradientChange = 255 - abs(ringGradient);
            for (int8_t strip = 0; strip < NumStrips; strip++)
            {
                active = true;
                uint8_t colorPosition = static_cast<uint8_t>(min(ringPercentDone * 255, 255));
                CRGB color = ColorFromPalette(PartyColors_p, colorPosition);
                display.leds[strip][led] = color;
                display.leds[strip][led].nscale8(ringGradientChange);
            }
        }

        // If all parts of the ring went off the end of the LED strips, mark it as inactive
        if (!active)
        {
            ringActive[r] = false;
        }
    }

    if (ringsActive < numDesiredRings && closestRing >= 0.5f)
    {
        ringActive[firstInactiveRing] = true;
        ringFireTimes[firstInactiveRing] = micros();
    }
}

void ShootAnimation::waves(uint16_t elapsed)
{
    const uint16_t PhaseStartTime = 10000;
    const uint16_t PhaseTime = 15000;
    const uint16_t FadeMs = 200;
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
        uint8_t sin = sin8(angle + incrementer + l);
        uint8_t strip;

        CRGB color;
        if (sin < 85)
        {
            strip = 0;
            color = (random8() < 200) ? CRGB::Blue : CRGB::DarkBlue;
        }
        else if (sin < 170)
        {
            strip = 1;
            color = (random8() < 200) ? CRGB::Fuchsia : CRGB::Purple;
        }
        else
        {
            strip = 2;
            color = (random8() < 200) ? CRGB::Cyan : CRGB::DarkSlateBlue;
        }

        display.leds[strip][l] = color;
    }

    EVERY_N_MILLISECONDS(1)
    {
        incrementer += 5;
    }
}

void ShootAnimation::done(uint16_t elapsed)
{
    const uint8_t GlowSize = 30;
    const uint16_t PhaseStartTime = 15000;
    const uint16_t PhaseTime = 17500;
    const uint16_t FadeMs = 500;
    const uint8_t FadeAmountPerFrame = min(255.0f / ((float)FadeMs / clock.getTargetMsPerFrame()), 255);

    if (elapsed < PhaseStartTime || elapsed >= PhaseTime)
    {
        return;
    }

    display.fadeToBlackBy(FadeAmountPerFrame);

    for (uint8_t l = 0; l < GlowSize; l++)
    {
        uint8_t level = static_cast<uint8_t>((GlowSize - l) / (float)GlowSize * 255.0f);

        for (uint8_t s = 0; s < NumStrips; s++)
        {
            if (random8() < 60)
            {
                display.leds[s][l] = CRGB::White;
                int16_t sparkleMod = level - random8(100);
                display.leds[s][l].nscale8(max(sparkleMod, 0));
            }
        }
    }
}

void ShootAnimation::sparkleAway(uint16_t elapsed)
{
    const uint16_t PhaseStartTime = 17000;
    const uint16_t PhaseTime = 23000;
    const uint16_t FadeMs = PhaseTime - PhaseStartTime;
    const uint8_t FadeAmountPerFrame = min(255.0f / ((float)FadeMs / clock.getTargetMsPerFrame()), 255);

    if (elapsed < PhaseStartTime || elapsed >= PhaseTime)
    {
        return;
    }

    display.fadeToBlackBy(FadeAmountPerFrame);

    uint16_t phaseElapsed = elapsed - PhaseStartTime;
    float percentDone = min(phaseElapsed / (float)FadeMs, 1.0f);
    uint8_t level = static_cast<uint8_t>(255.0f * (1.0f - percentDone));
    uint8_t swipe = min(static_cast<uint8_t>(NumLedsPerStrip * percentDone) * 3, NumLedsPerStrip);
    for (uint8_t l = 0; l < swipe; l++)
    {
        for (uint8_t s = 0; s < NumStrips; s++)
        {
            if (random8() < 64)
            {
                display.leds[s][l] = CRGB::White;
                int16_t sparkleMod = (level - 100) - random8(100);
                display.leds[s][l].nscale8(max(sparkleMod, 0));
            }
        }
    }
}

void ShootAnimation::loop()
{
    ulong elapsed = (micros() - startTime) / 1000;

    glowBase(elapsed);
    shots(elapsed);
    waves(elapsed);
    done(elapsed);
    sparkleAway(elapsed);
}

ulong ShootAnimation::animationTime() const
{
    return 24000;
}
