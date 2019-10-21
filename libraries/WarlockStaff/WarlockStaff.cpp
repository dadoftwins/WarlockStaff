#include "WarlockStaff.h"

#include <Utilities.h>

WarlockStaff::WarlockStaff() : 
    beatStripsAnimation(display, clock),
    glitterAnimation(display, clock),
    idleAnimation(display, clock),
    juggleDotsAnimation(display, clock),
    pulseFireAnimation(display, clock),
    rainbowAnimation(display, clock)
{
}

void WarlockStaff::setup()
{
    // Startup delay to give 45 seconds for re-uploading
    for (uint16_t i = 0; i < 45; i++)
    {
        delay(1000);
        if (Serial)
            break;
    }

    Serial.begin(115200);
    SerialPrintln("-=- Warlock|Staff -=- v1.0 (c) 2019 Tim Burrell");
    SerialPrintln("    Using [", NumStrips, "] Strips of [", NumLedsPerStrip, "] LEDS @ [", FPS, "] FPS");
    motion.setup();

    display.setup();
    clock.setup(FPS);
    
    beatStripsAnimation.setup();
    idleAnimation.setup(1000, 100, CRGB::White);
    glitterAnimation.setup(10, 888, 20, 200, CRGB::White, true);
    juggleDotsAnimation.setup();
    pulseFireAnimation.setup();
    rainbowAnimation.setup();

    currentAnimation = &pulseFireAnimation;
    scene = 3;

    animationFadeAmountPerFrame = 255 / (1000 / clock.getTargetMsPerFrame());

    SerialPrintln("Warlock|Staff setup complete");
}

void WarlockStaff::setAnimation(Animation* animation)
{
    SerialPrintln("Animation changing...");
    oldAnimation = currentAnimation;
    currentAnimation = animation;
    //currentAnimation->brightness = 0;
    SerialPrintln("Animation changed");
}

void WarlockStaff::loop()
{
    motion.loop();
    display.loop();
    currentAnimation->loop();

    // Update the clock and delay if needed in order to hit the target FPS
    clock.loop();

    EVERY_N_SECONDS(10)
    {
        scene = (scene + 1) % 6;
        SerialPrintln("Changin animation to: ", scene);

        switch (scene)
        {
        case 0:
            setAnimation(&idleAnimation);
            break;
        case 1:
            setAnimation(&glitterAnimation);
            break;
        case 2:
            setAnimation(&rainbowAnimation);
            break;
        case 3:
            setAnimation(&pulseFireAnimation);
            break;
        case 4:
            setAnimation(&beatStripsAnimation);
            break;
        case 5:
            setAnimation(&juggleDotsAnimation);
            break;
        }
    }
}