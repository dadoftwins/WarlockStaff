#ifndef _BeatStripsAnimation_h
#define _BeatStripsAnimation_h

#include <Dependencies.h>

#include <Animation.h>
#include <Clock.h>
#include <Display.h>

////////////////////////////////////////////////////////////////////////
// BeatStripsAnimation
//////////////////////////

class BeatStripsAnimation : public Animation
{
public:
    BeatStripsAnimation(Display& display, const Clock& clock) : Animation(display, clock)
    {
    }

    void setup();
    void loop();

private:
    uint8_t hue;
    uint8_t bpm;
};

#endif // _BeatStripsAnimation_h