#ifndef PMS_5003_SENSOR_H
#define PMS_5003_SENSOR_H

#include "PMS.h"


struct PMS5003Data {
    int pm2_5;
};

class PMS5003Sensor {
    public:
        PMS5003Sensor();    //constructor

        //Member functions
        void begin();
        PMS5003Data readData();
    private:
        PMS pms;    //PMS object
        PMS::DATA data; //variable of type PMS::DATA
};

#endif  //!PMS_5003_SENSOR_H