#include "WarlockStaff.h"

#include <Utilities.h>

WarlockStaff::WarlockStaff() : 
    beatStripsAnimation(display, clock),
    glitterAnimation(display, clock),
    idleAnimation(display, clock),
    juggleDotsAnimation(display, clock),
    pulseFireAnimation(display, clock),
    rainbowAnimation(display, clock),
    shootAnimation(display, clock)
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
    Serial1.begin(9600);
    SerialPrintln(F("-=- Warlock|Staff -=- v1.0"));
    SerialPrintln(F("    Using ["), NumStrips, F("] Strips of ["), NumLedsPerStrip, F("] LEDS @ ["), FPS, F("] FPS"));

    display.setup();
    clock.setup(FPS);
    
    beatStripsAnimation.setup();
    idleAnimation.setup(1000, 100, CRGB::White);
    glitterAnimation.setup(10, 888, 20, 200);
    juggleDotsAnimation.setup();
    pulseFireAnimation.setup();
    rainbowAnimation.setup();

    currentAnimation = &idleAnimation;
    idleScene = 0;
    state = StaffState::Idle;

    animationFadeAmountPerFrame = 255 / (1000 / clock.getTargetMsPerFrame());
}

void WarlockStaff::handleSenseEventsFomSerial()
{
    if (!Serial1.available())
    {
        return;
    }
    
    char c = Serial1.read();
    SerialPrintln(F("Sense received: ["), c, F("]: "), int(c));

    StaffState newState;
    switch (c)
    {
    case '0':
        newState = StaffState::Idle;
        break;
    case '1':
        newState = StaffState::Tap;
        break;
    case '2':
        newState = StaffState::DoubleTap;
        break;
    case '3':
        newState = StaffState::Horizontal;
        break;
    default:
        return;
    }

    if (newState == state)
    {
        return;
    }

    SerialPrintln(F("Sense state ["), state, F("] to ["), newState, F("]"));

    switch (newState)
    {
    case StaffState::Idle:
        idleScene = 0;
        break;
    case StaffState::Tap:
        break;
    case StaffState::DoubleTap:
        if (state != StaffState::Horizontal)
        {
            animationMinimumTime = pulseFireAnimation.animationTime();
            animationStartTime = micros();
            setAnimation(&pulseFireAnimation);
        }
        break;
    case StaffState::Horizontal:
        animationMinimumTime = shootAnimation.animationTime();
        animationStartTime = micros();
        shootAnimation.start();
        setAnimation(&shootAnimation);
        break;
    }

    state = newState;
}

void WarlockStaff::setAnimation(Animation* animation)
{
    oldAnimation = currentAnimation;
    currentAnimation = animation;
}

void WarlockStaff::loop()
{
    handleSenseEventsFomSerial();

    display.loop();
    currentAnimation->loop();

    // Update the clock and delay if needed in order to hit the target FPS
    clock.loop();

    // Check for priority animation
    if (animationMinimumTime > 0)
    {
        ulong activeTime = (micros() - animationStartTime) / 1000;
        if (activeTime < animationMinimumTime)
        {
            return;
        }

        animationMinimumTime = 0;
        idleScene = 0;
        setAnimation(&idleAnimation);   
    }

    EVERY_N_SECONDS(15)
    {
        if (state != StaffState::Idle)
        {
            return;
        }

        idleScene = (idleScene + 1) % 2;
        SerialPrintln(F("Idle anim -> "), idleScene);
        if (idleScene < 1)
        {
            setAnimation(&idleAnimation);
        }
        else
        {
            switch (random8(4))
            {
            case 0:
                setAnimation(&glitterAnimation);
                break;
            case 1:
                setAnimation(&rainbowAnimation);
                break;
            case 2:
                setAnimation(&beatStripsAnimation);
                break;
            case 4:
                setAnimation(&juggleDotsAnimation);
                break;
            }            
        }
    }
}