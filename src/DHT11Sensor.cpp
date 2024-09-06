/**
 * @file DHT11Sensor.cpp
 * @brief Implementation of the DHT11Sensor class for interfacing with the DHT11 sensor.
 * 
 * This file contains the implementation of the `DHT11Sensor` class, which provides methods
 * for initializing and reading data from a DHT11 temperature and humidity sensor.
 */

#include "DHT11Sensor.h"

/**
 * @brief Constructs a DHT11Sensor object and initializes the sensor.
 * 
 * The constructor initializes the DHT11 sensor with the specified pin and sensor type.
 * 
 * @param DHTPIN The GPIO pin number connected to the DHT11 sensor.
 * @param DHTTYPE The type of DHT sensor being used (DHT11, DHT22, etc.).
 */
DHT11Sensor::DHT11Sensor(uint8_t DHTPIN, uint8_t DHTTYPE) : dht(DHTPIN, DHTTYPE) {}

/**
 * @brief Initializes the DHT11 sensor.
 * 
 * This method initializes the DHT11 sensor by calling the `begin()` method of the
 * `DHT` library to start sensor operations.
 */
void DHT11Sensor::init() {
    dht.begin();  ///< Initialize the DHT11 sensor.
}

/**
 * @brief Reads temperature and humidity data from the DHT11 sensor.
 * 
 * This method reads temperature and humidity values from the DHT11 sensor. It uses
 * the `sensors_event_t` structure to store the sensor data and returns it as a
 * `DHT11Data` structure.
 * 
 * @return A `DHT11Data` structure containing the temperature and humidity readings.
 */
DHT11Data DHT11Sensor::readDHT11() {
    sensors_event_t event; ///< Declare a variable to hold sensor reading data
    DHT11Data data;        ///< Structure to hold the temperature and humidity data

    dht.temperature().getEvent(&event);       ///< Get the temperature reading
    data.temperature = event.temperature;     ///< Store the temperature value

    dht.humidity().getEvent(&event);          ///< Get the humidity reading
    data.humidity = event.relative_humidity;  ///< Store the humidity value

    return data;  ///< Return the data structure with temperature and humidity readings
}
