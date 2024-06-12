#include <Arduino.h>

class BLDC{
public:
    BLDC(uint8_t PWMPin){
        ledcAttachPin(PWMPin, PWMChannel);
        ledcAttachPin(PWMPin, PWMChannel);
    }

    void control_motor(){
        Serial.println("Motor started....");
    }

private:
    /*Setting PWM Parameters*/
    static const int PWMFreq = 20000;
    static const int PWMChannel = 0;
    static const int PWMResolution = 11;

};