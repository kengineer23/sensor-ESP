#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

typedef struct DHT11Data{
    float temperature;
    float humidity;
};

class DHT11Sensor {
public:
    // 'DHTPIN' specifies the pin number to which the sensor is connected.
    // 'DHTTYPE' specifies the type of DHT sensor (e.g., DHT11, DHT22).
    DHT11Sensor(uint8_t DHTPIN, uint8_t DHTTYPE) : dht(DHTPIN,DHTTYPE){}    //constructor : initializer list

    void begin(){
        dht.begin();
    }

    DHT11Data readDHT11(){
        // Declare a variable 'event' of type 'sensors_event_t' to hold sensor reading data
        sensors_event_t event;
        DHT11Data data;
        dht.temperature().getEvent(&event);
        data.temperature = event.temperature;
        data.humidity = event.relative_humidity;

        return data;
    }
private:
  // Create a DHT_Unified object named 'dht' to interface with a DHT sensor.
  DHT_Unified dht;
};