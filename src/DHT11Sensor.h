#ifndef DHT_11_SENSOR_H
#define DHT_11_SENSOR_H

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

struct DHT11Data{
    float temperature;
    float humidity;
};

class DHT11Sensor {
    public:
        // 'DHTPIN' specifies the pin number to which the sensor is connected.
        // 'DHTTYPE' specifies the type of DHT sensor (e.g., DHT11, DHT22).
        DHT11Sensor(uint8_t DHTPIN, uint8_t DHTTYPE);   //constructor

        //Member functions
        void init();
        DHT11Data readDHT11();

    private:
    // Create a DHT_Unified object named 'dht' to interface with a DHT sensor.
    DHT_Unified dht;
};

#endif  //!DHT_11_SENSOR_H