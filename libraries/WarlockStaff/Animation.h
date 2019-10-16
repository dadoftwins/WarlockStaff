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
    Animation(const Display& systemDisplay, const Clock& systemClock) : 
        display(systemDisplay), 
        clock(systemClock),
        brightness(0xff)
    {
    }

    virtual void loop() = 0;

    uint8_t brightness;

protected:
    const Display& display;
    const Clock& clock;
};

#endif // _Animation_h