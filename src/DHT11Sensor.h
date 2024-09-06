/**
 * @file DHT11Sensor.h
 * @brief Header file for the DHT11Sensor class.
 * 
 * This header file defines the `DHT11Sensor` class for interfacing with a DHT11 temperature
 * and humidity sensor. It includes the class declaration, a data structure for holding sensor
 * readings, and method declarations for initializing and reading from the sensor.
 */

#ifndef DHT_11_SENSOR_H
#define DHT_11_SENSOR_H

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

/**
 * @struct DHT11Data
 * @brief Structure to hold temperature and humidity data from the DHT11 sensor.
 * 
 * This structure holds the temperature and humidity readings obtained from the DHT11 sensor.
 */
struct DHT11Data {
    float temperature; ///< Temperature reading from the DHT11 sensor in degrees Celsius.
    float humidity;    ///< Humidity reading from the DHT11 sensor in percentage.
};

/**
 * @class DHT11Sensor
 * @brief A class for interfacing with the DHT11 temperature and humidity sensor.
 * 
 * The `DHT11Sensor` class provides methods to initialize and read data from the DHT11 sensor.
 * It uses the Adafruit DHT library to interface with the sensor and retrieve temperature and
 * humidity readings.
 */
class DHT11Sensor {
    public:
        /**
         * @brief Constructs a DHT11Sensor object and initializes the sensor interface.
         * 
         * The constructor initializes the DHT11 sensor with the specified GPIO pin and sensor type.
         * 
         * @param DHTPIN The GPIO pin number connected to the DHT11 sensor.
         * @param DHTTYPE The type of DHT sensor being used (e.g., DHT11, DHT22).
         */
        DHT11Sensor(uint8_t DHTPIN, uint8_t DHTTYPE); ///< Constructor

        /**
         * @brief Initializes the DHT11 sensor.
         * 
         * This method sets up the DHT11 sensor by calling the `begin()` method of the `DHT` library
         * to start sensor operations.
         */
        void init(); ///< Initializes the DHT11 sensor.

        /**
         * @brief Reads temperature and humidity data from the DHT11 sensor.
         * 
         * This method retrieves temperature and humidity values from the DHT11 sensor. It returns
         * the data in a `DHT11Data` structure.
         * 
         * @return A `DHT11Data` structure containing the temperature and humidity readings.
         */
        DHT11Data readDHT11(); ///< Reads temperature and humidity data from the DHT11 sensor.

    private:
        /** @brief DHT_Unified object for interfacing with the DHT sensor. */
        DHT_Unified dht; ///< DHT sensor interface object.
};

#endif  //!DHT_11_SENSOR_H
