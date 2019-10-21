#ifndef _IdleAnimation_h
#define _IdleAnimation_h

#include <Dependencies.h>

#include <Animation.h>
#include <Clock.h>
#include <Display.h>
#include <EasingLib.h>

////////////////////////////////////////////////////////////////////////
// IdleAnimation
//////////////////////////

class IdleAnimation : public Animation
{
public:
    IdleAnimation(Display& display, const Clock& clock) : Animation(display, clock)
    {
    }

    void setup(uint16_t msToSweep = 1000, uint16_t msToFade = 100, CRGB color = CRGB::White);
    void loop();

private:
    Easing easing;
    uint8_t fadeAmountPerFrame = 255;
    bool direction = true;
    CRGB color;
};

#endif // _IdleAnimation_h