#ifndef _ShootAnimation_h
#define _ShootAnimation_h

#include <Dependencies.h>

#include <Animation.h>
#include <Clock.h>
#include <Display.h>

////////////////////////////////////////////////////////////////////////
// ShootAnimation
//////////////////////////

class ShootAnimation : public Animation
{
public:
    ShootAnimation(Display& display, const Clock& clock) : Animation(display, clock)
    {
    }

    void setup();
    void loop();
    virtual ulong animationTime() const override;
    virtual void start() override;

private:
    uint16_t frame = 0;
};

#endif // _ShootAnimation_h