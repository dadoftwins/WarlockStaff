#ifndef _RainbowAnimation_h
#define _RainbowAnimation_h

#include <Dependencies.h>

#include <Animation.h>
#include <Clock.h>
#include <Display.h>
#include <EasingLib.h>

////////////////////////////////////////////////////////////////////////
// RainbowAnimation
//////////////////////////

class RainbowAnimation : public Animation
{
public:
    RainbowAnimation(const Display& display, const Clock& clock) : Animation(display, clock)
    {
    }

    void setup();
    void loop();

private:
    uint8_t hue;
};

#endif // _RainbowAnimation_h