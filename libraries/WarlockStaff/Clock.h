#ifndef _Clock_h
#define _Clock_h

#include <Dependencies.h>

////////////////////////////////////////////////////////////////////////
// Clock Class
//////////////////////////

class Clock
{
public:
    void setup(uint16_t fps);
    void loop();

    void setFPS(uint16_t fps);
    uint16_t getTargetMsPerFrame() const;

private:
    ulong lastUpdate;
    uint8_t targetMsPerFrame;
    uint16_t frameCounter = 0;
    uint16_t millisSinceUpdate = 0;
    uint16_t delayed = 0;
};

#endif // _Clock_h