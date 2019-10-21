#ifndef _GlitterAnimation_h
#define _GlitterAnimation_h

#include <Dependencies.h>

#include <Animation.h>
#include <Clock.h>
#include <Display.h>

////////////////////////////////////////////////////////////////////////
// GlitterAnimation
//////////////////////////

class GlitterAnimation : public Animation
{
public:
    GlitterAnimation(Display& display, const Clock& clock) : Animation(display, clock)
    {
    }

    void setup(uint16_t popInMs = 100,
        uint16_t fadeMs = 100,
        uint8_t maxSparkles = 20,
        uint8_t chanceOfSparkle = 200,
        CRGB sparkleColor = CRGB::White,
        bool randomColor = false);

    void loop();

private:
    uint16_t popInMs = 0;
    uint16_t fadeMs = 0;
    uint8_t maxSparkles = 0;
    uint8_t curSparkles = 0;
    uint8_t chanceOfSparkle = 0;
    uint8_t fadeAmountPerFrame = 0;
    CRGB color;
    bool randomColor;
};

#endif // _GlitterAnimation_h