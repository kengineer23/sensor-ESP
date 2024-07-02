#ifndef MQ_2_SENSOR_H
#define MQ_2_SENSOR_H


struct MQ2Data{
    int gasValue;
};


class MQ2Sensor{
public:
    MQ2Sensor(uint8_t pin);

    //Member function(s)
    MQ2Data gasRead();
private:
    uint8_t gaspin;
};

#endif // !MQ_2_SENSOR_H