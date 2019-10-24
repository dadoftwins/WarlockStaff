#include "ShootAnimation.h"

#include <avr/pgmspace.h>

void ShootAnimation::setup()
{
}

void ShootAnimation::start()
{
}

void ShootAnimation::loop()
{
    display.fadeToBlackBy(1);
}

ulong ShootAnimation::animationTime() const
{
    return 10000;
}
