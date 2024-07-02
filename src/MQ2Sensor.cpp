#include "MQ2Sensor.h"

#include "Arduino.h"

MQ2Sensor::MQ2Sensor(uint8_t pin) : gaspin(pin){   //constructor : initializer list
        pinMode(gaspin, INPUT);
    }

MQ2Data MQ2Sensor::gasRead(){
    MQ2Data data;
    data.gasValue = digitalRead(gaspin);

    return data;
}

