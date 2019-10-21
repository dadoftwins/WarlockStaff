#ifndef _Motion_h
#define _Motion_h

#include <Dependencies.h>
#include <SoftwareSerial.h>

////////////////////////////////////////////////////////////////////////
// Motion
//////////////////////////

#define MotionSerialBufLen 16

class Motion
{
public:
    Motion();

    void setup();
    void loop();

private:
    SoftwareSerial comm;
    uint8_t length;
    char buf[MotionSerialBufLen];
};

#endif // _Motion_h