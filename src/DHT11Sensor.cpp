#include "DHT11Sensor.h"


DHT11Sensor::DHT11Sensor(uint8_t DHTPIN, uint8_t DHTTYPE):dht(DHTPIN,DHTTYPE){}    //constructor : initializer list


void DHT11Sensor::init(){
    dht.begin();
}

DHT11Data DHT11Sensor::readDHT11(){
        // Declare a variable 'event' of type 'sensors_event_t' to hold sensor reading data
        sensors_event_t event;
        DHT11Data data;
        dht.temperature().getEvent(&event);
        data.temperature = event.temperature;
        dht.humidity().getEvent(&event);
        data.humidity = event.relative_humidity;
        return data;
}