#ifndef _WarlockStaff_h
#define _WarlockStaff_h

#include <Dependencies.h>

#include <BeatStripsAnimation.h>
#include <Clock.h>
#include <Display.h>
#include <GlitterAnimation.h>
#include <IdleAnimation.h>
#include <JuggleDotsAnimation.h>
#include <PulseFireAnimation.h>
#include <RainbowAnimation.h>

////////////////////////////////////////////////////////////////////////
// WarlockStaff Class
//////////////////////////

enum StaffState
{
    Idle,
    Tap,
    DoubleTap,
    Horizontal
};

class WarlockStaff
{
public:
    WarlockStaff();

    void setup();
    void loop();

    void setAnimation(Animation* animation);

private:
    void handleSenseEvents();

    BeatStripsAnimation beatStripsAnimation;
    Clock clock;
    Display display;
    GlitterAnimation glitterAnimation;
    IdleAnimation idleAnimation;
    JuggleDotsAnimation juggleDotsAnimation;
    PulseFireAnimation pulseFireAnimation;
    RainbowAnimation rainbowAnimation;
    uint8_t scene = 0;
    uint8_t animationFadeAmountPerFrame;
    Animation* currentAnimation = nullptr;
    Animation* oldAnimation = nullptr;
    StaffState state = StaffState::Idle;
    StaffState debounceState = StaffState::Idle;
    ulong debounceTimer;
};

#endif // _WarlockStaff_h