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
    void glowBase(uint16_t elapsedMs);
    void waves(uint16_t elapsedMs);
    void shots(uint16_t elapsedMs);
    void done(uint16_t elapsedMs);
    void sparkleAway(uint16_t elapsedMs);

    static const uint8_t MaxRings = 10;

    uint16_t frame = 0;
    uint8_t phase = 0;
    ulong startTime = 0;
    uint16_t incrementer = 0;
    ulong ringFireTimes[MaxRings];
    bool ringActive[MaxRings] = {false};
};

#endif // _ShootAnimation_h