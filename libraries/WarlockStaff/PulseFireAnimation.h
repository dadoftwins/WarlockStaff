#ifndef _PulseFireAnimation_h
#define _PulseFireAnimation_h

#include <Dependencies.h>

#include <Animation.h>
#include <Clock.h>
#include <Display.h>
#include <EasingLib.h>

////////////////////////////////////////////////////////////////////////
// PulseFireAnimation
//////////////////////////

class PulseFireAnimation : public Animation
{
public:
    PulseFireAnimation(const Display& display, const Clock& clock) : Animation(display, clock)
    {
    }

    void setup();
    void loop();

private:
    void fireOnStrip(uint8_t strip);

    CRGBPalette16 pallette;
    bool reverseDirection;
    int cooling;
    int sparking;
};

#endif // _PulseFireAnimation_h