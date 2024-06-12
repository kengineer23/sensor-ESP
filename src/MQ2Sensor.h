#include <Arduino.h>

typedef struct MQ2Data{
    int gasValue;
};


class MQ2Sensor{
public:
    MQ2Sensor(uint8_t pin) : gaspin(pin){   //constructor : initializer list
        pinMode(gaspin, INPUT);
    }

    MQ2Data gasRead(){
        MQ2Data data;
        data.gasValue = digitalRead(gaspin);

        return data;
        }
private:
    uint8_t gaspin;
};