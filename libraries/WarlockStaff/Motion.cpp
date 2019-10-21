#include "Motion.h"

Motion::Motion() :
    comm(CommRxPin, CommTxPin)
{
}

void Motion::setup()
{
    Serial.println("Motion setup...");

    length = 0;
    memset(buf, 0, MotionSerialBufLen);
    comm.begin(9600);
}

void Motion::loop()
{
    if (!comm.available())
    {
        return;
    }

    char c = comm.read();
    Serial.print("CommDebug: ");
    Serial.print((int)c);
    Serial.println();

    if (c == 13 || length == MotionSerialBufLen - 1)
    {
        Serial.print("CommPort: ");
        Serial.println(buf);
        length = 0;
        memset(buf, 0, MotionSerialBufLen);
    }

    buf[length++] = c;
}
