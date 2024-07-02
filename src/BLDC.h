#include <Arduino.h>

class BLDC{
public:
    BLDC(uint8_t PWMPin){
        ledcSetup(PWMChannel, PWMFreq, PWMResolution);
        ledcAttachPin(PWMPin, PWMChannel);
    }

    void control_motor(){
        Serial.println("Motor started....");
        for(int dutyCycle = 0; dutyCycle <= 2047; dutyCycle++)
        {
            ledcWrite(PWMChannel, dutyCycle);
            delay(10);
        }
    }

private:
    /*Setting PWM Parameters*/
    static const int PWMFreq = 20000;
    static const int PWMChannel = 0;
    static const int PWMResolution = 11;

};