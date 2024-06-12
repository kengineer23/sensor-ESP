#include "PMS.h"
#include "Arduino.h"

typedef struct PMS5003Data {
    int pm2_5;
};

class PMS5003Sensor {
public:
    PMS5003Sensor() : pms(Serial2) {}

    void begin() {
        Serial2.begin(9600);  //UART2 Channel of ESP32(Rx=16, Tx=17)
        pms.activeMode();    // Switch to active mode
        pms.wakeUp();
    }

    PMS5003Data readData(){
        PMS5003Data pmsdata;
        if(pms.readUntil(data)){
            pmsdata.pm2_5 = data.PM_AE_UG_2_5;
        }

        return pmsdata;
    }
private:
    PMS pms;    //PMS object
    PMS::DATA data; //variable of type PMS::DATA
};