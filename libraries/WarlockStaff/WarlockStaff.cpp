#include "WarlockStaff.h"

WarlockStaff::WarlockStaff() : 
    idleAnimation(display, clock),
    glitterAnimation(display, clock),
    rainbowAnimation(display, clock)
{
}

void WarlockStaff::setup()
{
    while (!Serial) {};
    Serial.begin(115200);
    Serial.println("-=- Warlock|Staff -=- v0.1 (c) 2019 Tim Burrell");

    display.setup();
    clock.setup(FPS);
    
    idleAnimation.setup(1000, 100, CRGB::White);
    glitterAnimation.setup(10, 888, 20, 200, CRGB::White, true);
    rainbowAnimation.setup();

    currentAnimation = &rainbowAnimation;

    animationFadeAmountPerFrame = 255 / (1000 / clock.getTargetMsPerFrame());
    Serial.println("Warlock|Staff setup complete");
}

void WarlockStaff::setAnimation(Animation* animation)
{
    Serial.println("Animation changing...");
    oldAnimation = currentAnimation;
    currentAnimation = animation;
    //currentAnimation->brightness = 0;
    Serial.println("Animation changed");
}

void WarlockStaff::loop()
{
    display.loop();

    // if (oldAnimation && oldAnimation->brightness > 0)
    // {
    //     oldAnimation->loop();

    //     oldAnimation->brightness = (oldAnimation->brightness >= animationFadeAmountPerFrame) ? oldAnimation->brightness - animationFadeAmountPerFrame : 0;
    //     Serial.print("Fading old animation out: ");
    //     Serial.println(oldAnimation->brightness);
    // }

    // if (currentAnimation->brightness < 0xff)
    // {
    //     currentAnimation->brightness = (currentAnimation->brightness < 0xff - animationFadeAmountPerFrame) ? currentAnimation->brightness + animationFadeAmountPerFrame : 0xff;
    //     Serial.print("Fading new animation in: ");
    //     Serial.println(currentAnimation->brightness);
    // }

    currentAnimation->loop();

    // Update the clock and delay if needed in order to hit the target FPS
    clock.loop();

    EVERY_N_SECONDS(30)
    {
        scene = (scene + 1) % 3;
        Serial.print("Changin animation to: ");
        Serial.println(scene);

        switch (scene)
        {
        case 0:
            setAnimation(&idleAnimation);
            break;
        case 1:
            setAnimation(&glitterAnimation);
            break;
        case 3:
            setAnimation(&rainbowAnimation);
            break;
        }
    }
}