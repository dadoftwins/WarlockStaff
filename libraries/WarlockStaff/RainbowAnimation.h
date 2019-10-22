#ifndef _RainbowAnimation_h
#define _RainbowAnimation_h

#include <Dependencies.h>

#include <Animation.h>
#include <Clock.h>
#include <Display.h>

////////////////////////////////////////////////////////////////////////
// RainbowAnimation
//////////////////////////

class RainbowAnimation : public Animation
{
public:
    RainbowAnimation(Display& display, const Clock& clock) : Animation(display, clock)
    {
    }

    void setup();
    void loop();

private:
    uint8_t hue;
    bool ocean = true;
    bool direction = true;
};

#endif // _RainbowAnimation_h