////////////////////////////////////////////////////////////////////////
// -=- Warlock|Staff|Sense -=- (c) Tim Burrell - 2019
//////////////////////////

#include <Arduino.h>
#include <Arduino_LSM9DS1.h>

////////////////////////////////////////////////////////////////////////
// Functions
//////////////////////////

void setup()
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

    while (true)
    {
        if (!IMU.begin())
        {
            Serial.println("Failed to initialize IMU!");
            delay(1000);
            continue;
        }
     
        break;
    }

    Serial.print("Accelerometer sample rate = ");
    Serial.println(IMU.accelerationSampleRate());

    Serial.print("Gyroscope sample rate = ");
    Serial.println(IMU.gyroscopeSampleRate());
}

void loop()
{
    float x, y, z;

    if (IMU.accelerationAvailable())
    {
        IMU.readAcceleration(x, y, z);

        Serial.print("Accel: ");
        Serial.print(x);
        Serial.print('\t');
        Serial.print(y);
        Serial.print('\t');
        Serial.println(z);

        Serial1.print("a");
        Serial1.print(x);
        Serial1.print(" ");
        Serial1.print(y);
        Serial1.print(" ");
        Serial1.println(z);
    }

    if (IMU.gyroscopeAvailable())
    {
        IMU.readGyroscope(x, y, z);

        Serial.print(" Gyro: ");
        Serial.print(x);
        Serial.print('\t');
        Serial.print(y);
        Serial.print('\t');
        Serial.println(z);

        Serial1.print("g");
        Serial1.print(x);
        Serial1.print(" ");
        Serial1.print(y);
        Serial1.print(" ");
        Serial1.println(z);
    }
}