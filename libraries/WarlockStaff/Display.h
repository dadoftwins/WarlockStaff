#ifndef _Display_h
#define _Display_h

#include <Dependencies.h>

////////////////////////////////////////////////////////////////////////
// Display Class
//////////////////////////

class Display
{
public:
    void setup();
    void loop();
   
    void clear(bool refresh=true);
    void fadeToBlackBy(uint8_t amount);

    CRGB leds[NumStrips][NumLedsPerStrip];
};

#endif // _Display_h