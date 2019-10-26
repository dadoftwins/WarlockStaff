#ifndef _JuggleDotsAnimation_h
#define _JuggleDotsAnimation_h

#include <Dependencies.h>

#include <Animation.h>
#include <Clock.h>
#include <Display.h>

////////////////////////////////////////////////////////////////////////
// JuggleDotsAnimation
//////////////////////////

class JuggleDotsAnimation : public Animation
{
public:
    JuggleDotsAnimation(Display& display, const Clock& clock) : Animation(display, clock)
    {
    }

    void setup();
    void loop();

private:
    uint8_t hue;
};

#endif // _JuggleDotsAnimation_h