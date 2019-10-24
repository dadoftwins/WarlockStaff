#ifndef _Animation_h
#define _Animation_h

#include <Clock.h>
#include <Display.h>

////////////////////////////////////////////////////////////////////////
// Animation Base Class
//////////////////////////

class Animation
{
public:
    Animation(Display& systemDisplay, const Clock& systemClock) : 
        display(systemDisplay), 
        clock(systemClock),
        brightness(0xff)
    {
    }

    virtual void loop() = 0;
    virtual ulong animationTime() const { return 0; }
    virtual void start() {}

    uint8_t brightness;

protected:
    Display& display;
    const Clock& clock;
};

#endif // _Animation_h