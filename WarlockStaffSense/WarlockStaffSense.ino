////////////////////////////////////////////////////////////////////////
// -=- Warlock|Staff|Sense -=- (c) Tim Burrell - 2019
//////////////////////////

#include <Arduino.h>
#include <Arduino_LSM9DS1.h>

#define ulong unsigned long

////////////////////////////////////////////////////////////////////////
// Settings
//////////////////////////

const uint16_t FPS = 240; // Refresh rate of sensor is 120Hz, go double that
const uint8_t DelayPerLoop = 1000 / FPS;
const float TapGsThreshold = 5.0f;
const uint16_t DoubleTapMs = 2000;
const uint16_t DoubleTapDelayMs = 300;
const float HorizontalAngleTresholdEnable = 1.0f;
const float HorizontalAngleTresholdDisable = 0.5f;
const uint8_t BaselineReadings = 10;
const uint16_t HorizontalDebounceMs = 750;
const uint16_t HorizontalChangeMs = 1000;

#define OutPin0 12
#define OutPin1 11

////////////////////////////////////////////////////////////////////////
// Vector3 class
//////////////////////////

class Vector3
{
public:
    Vector3();
    Vector3(float x, float y, float z);
    bool operator==(const Vector3& rhs) const;
    Vector3 operator+(const Vector3& rhs) const;
    Vector3 operator+=(const Vector3& rhs);
    Vector3 operator-(const Vector3& rhs) const;
    Vector3 operator*(float scalar) const;
    Vector3 operator/(float scalar) const;
    Vector3 operator/=(float scalar);
    Vector3 cross(const Vector3& rhs) const;
    float dot(const Vector3& rhs) const;
    float length() const;
    float angle(const Vector3& rhs) const;

    float x;
    float y;
    float z;
};

Vector3::Vector3() : x(0), y(0), z(0)
{
}

Vector3::Vector3(float initX, float initY, float initZ) : x(initX), y(initY), z(initZ)
{
}

bool Vector3::operator==(const Vector3& rhs) const
{
    return(x == rhs.x && y == rhs.y && z == rhs.z);
}

Vector3 Vector3::operator+(const Vector3& rhs) const
{
    return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
}

Vector3 Vector3::operator+=(const Vector3& rhs)
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;

    return *this;
}

Vector3 Vector3::operator-(const Vector3& rhs) const
{
    return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
}

Vector3 Vector3::operator/(float scalar) const
{
    return Vector3(x / scalar, y / scalar, z / scalar);
}

Vector3 Vector3::operator/=(float scalar)
{
    x /= scalar;
    y /= scalar;
    z /= scalar;

    return *this;
}

float Vector3::dot(const Vector3& rhs) const
{
    return (x * rhs.x + y * rhs.y + z * rhs.z);
}

Vector3 Vector3::cross(const Vector3& rhs) const
{
    return Vector3(y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x);
}

float Vector3::length() const
{
    return float(sqrt(x*x + y*y + z*z));
}

float Vector3::angle(const Vector3& rhs) const
{
    float dotProduct = dot(rhs);
    float len1 = length();
    float len2 = rhs.length();
    float angle = acos(dotProduct / sqrt(len1 * len2));
    return angle;    
}

////////////////////////////////////////////////////////////////////////
// Main class
//////////////////////////

enum StaffState
{
    Idle,
    Tap,
    DoubleTap,
    Horizontal
};

class WarlockStaffSense
{
public:
    void setup();
    void loop();
    void updateAcceleration(const Vector3& accel);
    void updateGyroscope(const Vector3& gyroscope);
    void setState(StaffState state);

private:
    Vector3 accel;
    Vector3 gyro;
    Vector3 baselineReading;
    ulong baselineReadings = 0;
    bool firstAccel = true;
    bool firstGyro = true;
    ulong firstTap;
    ulong timeInState;
    ulong timeInHorizontalState;
    StaffState horizontalState = StaffState::Idle;
    StaffState state = StaffState::Idle;
};

void WarlockStaffSense::setup()
{
    // Startup delay to give 90 seconds for re-uploading
    for (uint16_t i = 0; i < 90; i++)
    {
        delay(1000);
        if (Serial)
            break;
    }
    Serial.begin(115200);
    Serial.println("-=- Warlock|Staff|Sense -=- v1.0 (c) 2019 Tim Burrell");
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

    firstTap = micros();

    pinMode(OutPin0, OUTPUT);
    pinMode(OutPin1, OUTPUT);

    state = StaffState::Tap;
    setState(StaffState::Idle);
}

void WarlockStaffSense::setState(StaffState newState)
{
    if (newState != horizontalState)
    {
        timeInHorizontalState = micros();
        horizontalState = newState;
    }

    if (newState == state)
    {
        return;
    }

    if (newState == StaffState::Horizontal || state == StaffState::Horizontal)
    {
        ulong time = (micros() - timeInHorizontalState) / 1000;
        if (time < HorizontalDebounceMs)
        {
            return;
        }

        time = (micros() - timeInState) / 1000;
        if (time < HorizontalChangeMs)
        {
            return;
        }
    }

    timeInState = micros();
    timeInHorizontalState = timeInState;

    Serial.print("State changing from [");
    Serial.print(state);
    Serial.print("] -> [");
    Serial.print(newState);
    Serial.println("]");

    state = newState;
    horizontalState = newState;

    switch (state)
    {
    case StaffState::Idle:
        Serial1.write('0');
        break;
    case StaffState::Tap:
        Serial1.write('1');
        break;
    case StaffState::DoubleTap:
        Serial1.write('2');
        break;
    case StaffState::Horizontal:
        Serial1.write('3');
        break;
    }
}

void WarlockStaffSense::updateAcceleration(const Vector3& reading)
{
    if (firstAccel)
    {
        accel = reading;
        baselineReading = reading;
        firstAccel = false;
        return;
    }

    if (baselineReadings < BaselineReadings)
    {
        baselineReadings++;
    }
    else if (baselineReadings == BaselineReadings)
    {
        Serial.println("Baseline reading completed");
        baselineReadings++;
        baselineReading = reading;
    }
    else
    {
        // Check rotation
        float angle = baselineReading.angle(reading);
        if (state != StaffState::Horizontal)
        {
            if (abs(angle) >= HorizontalAngleTresholdEnable)
            {
                setState(StaffState::Horizontal);
                accel = reading;
                return;
            }
        }
        else if (abs(angle) >= HorizontalAngleTresholdDisable)
        {
            accel = reading;
            return;
        }            
    }

    // Look for taps
    float strength = (reading - accel).length();
    if (strength >= TapGsThreshold)
    {
        Serial.print("Tap: ");
        Serial.println(strength);

        switch (state)
        {
        case StaffState::Horizontal:
        case StaffState::Idle:
            firstTap = micros();
            setState(StaffState::Tap);
            break;
        case StaffState::Tap:
            if ((micros() - firstTap) / 1000 <= DoubleTapMs && 
                (micros() - firstTap) / 1000 >= DoubleTapDelayMs)
            {
                setState(StaffState::DoubleTap);
            }
            break;
        default:
            // do mothing
            Serial.println("Tap detection -- nothing to do");
            break;
        }

        accel = reading;
        return;
    }

    ulong timeSinceFirstTap = (micros() - firstTap) / 1000;
    if ((state == StaffState::Tap || state == StaffState::DoubleTap) && timeSinceFirstTap <= DoubleTapMs)
    {
        accel = reading;
        return;
    }

    setState(StaffState::Idle);
}

void WarlockStaffSense::updateGyroscope(const Vector3& reading)
{
    if (firstGyro)
    {
        gyro = reading;
        firstGyro = false;
        return;
    }

    gyro = reading;
}

void WarlockStaffSense::loop()
{
    Vector3 reading;

    if (IMU.accelerationAvailable())
    {
        IMU.readAcceleration(reading.x, reading.y, reading.z);
        updateAcceleration(reading);
    }

    if (IMU.gyroscopeAvailable())
    {
        IMU.readGyroscope(reading.x, reading.y, reading.z);
        updateGyroscope(reading);
    }

    EVERY_N_MILLISECONDS(1000)
    {
        Serial1.println("!");
    }

    // Save power
    delay(DelayPerLoop);   
}

WarlockStaffSense warlockStaffSense;

////////////////////////////////////////////////////////////////////////
// Functions
//////////////////////////

void setup()
{
    warlockStaffSense.setup();
}

void loop()
{
    warlockStaffSense.loop();
}