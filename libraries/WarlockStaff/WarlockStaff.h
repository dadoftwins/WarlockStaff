#ifndef _WarlockStaff_h
#define _WarlockStaff_h

#include <Dependencies.h>

#include <Clock.h>
#include <Display.h>
#include <GlitterAnimation.h>
#include <IdleAnimation.h>
#include <RainbowAnimation.h>

////////////////////////////////////////////////////////////////////////
// WarlockStaff Class
//////////////////////////

class WarlockStaff
{
public:
    WarlockStaff();

    void setup();
    void loop();

    void setAnimation(Animation* animation);


private:
    Clock clock;
    Display display;
    GlitterAnimation glitterAnimation;
    IdleAnimation idleAnimation;
    RainbowAnimation rainbowAnimation;
    uint8_t scene = 0;
    uint8_t animationFadeAmountPerFrame;
    Animation* currentAnimation = nullptr;
    Animation* oldAnimation = nullptr;
};

#endif // _WarlockStaff_h