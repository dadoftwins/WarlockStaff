#include "Clock.h"

void Clock::setup(uint16_t fps)
{
    setFPS(fps);
    lastUpdate = millis();
}

void Clock::loop()
{
    frameCounter++;
    
    ulong now = millis();
    ulong elapsed = now - lastUpdate;
    millisSinceUpdate += elapsed;
    if (millisSinceUpdate >= 1000)
    {
        Serial.print("Clock FPS: ");
        Serial.print(frameCounter);
        Serial.print(" delayed ");
        Serial.print(delayed);
        Serial.println("ms");
        frameCounter = 0;
        millisSinceUpdate = 0;
        delayed = 0;
    }

    int msToDelay = targetMsPerFrame - elapsed;
    if (msToDelay > 0)
    {
        delayed += msToDelay;
        delay(msToDelay);
    }

    lastUpdate = now;
}

void Clock::setFPS(uint16_t fps)
{
    Serial.print("Clock FPS set to: ");
    Serial.print(fps);
    
    targetMsPerFrame = 1750 / fps;

    Serial.print(" targetMsPerFrame: ");
    Serial.println(targetMsPerFrame);
}

uint16_t Clock::getTargetMsPerFrame()
{
    return targetMsPerFrame;
}