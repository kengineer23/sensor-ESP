#include "PMS5003Sensor.h"
#include "Arduino.h"

PMS5003Sensor::PMS5003Sensor():pms(Serial2){}

void PMS5003Sensor::begin() {
        Serial2.begin(9600);  //UART2 Channel of ESP32(Rx=16, Tx=17)
        pms.activeMode();    // Switch to active mode
        pms.wakeUp();
    }

PMS5003Data PMS5003Sensor::readData(){
    PMS5003Data pmsdata;
    if(pms.readUntil(data)){
        pmsdata.pm2_5 = data.PM_AE_UG_2_5;
        Serial.println(pmsdata.pm2_5);
    }

    return pmsdata;
}