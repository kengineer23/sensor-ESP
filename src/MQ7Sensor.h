/**
 * @file MQ7Sensor.h
 * @brief Header file for the MQ7Sensor class.
 * 
 * This header file defines the `MQ7Sensor` class, which provides methods for interfacing
 * with the MQ7 gas sensor. It includes the class constructor and member functions
 * for reading gas concentration data.
 */

#ifndef MQ_7_SENSOR_H
#define MQ_7_SENSOR_H

#include <cstdint> // For uint8_t

/**
 * @struct MQ7Data
 * @brief Structure to hold MQ7 gas sensor data.
 * 
 * This structure contains the measurement data from the MQ7 gas sensor. Currently,
 * it holds the gas concentration value read from the sensor.
 */
struct MQ7Data {
    int gasValue; ///< Gas concentration value read from the sensor.
};

/**
 * @class MQ7Sensor
 * @brief A class for interfacing with the MQ7 gas sensor.
 * 
 * The `MQ7Sensor` class provides methods to initialize the MQ7 sensor and read gas
 * concentration data from it. The class uses a digital pin to communicate with the sensor.
 */
class MQ7Sensor {
public:
    /**
     * @brief Constructs an MQ7Sensor object.
     * 
     * The constructor initializes the `MQ7Sensor` object by setting up the pin used
     * for interfacing with the MQ7 sensor. The pin is configured as an input to read
     * digital values from the sensor.
     * 
     * @param pin The pin number to which the MQ7 sensor is connected.
     */
    MQ7Sensor(uint8_t pin); ///< Constructor declaration.

    /**
     * @brief Reads gas concentration data from the MQ7 sensor.
     * 
     * This method retrieves the gas concentration value from the MQ7 sensor. It reads
     * a digital value from the specified sensor pin and returns it in an `MQ7Data` structure.
     * 
     * @return An `MQ7Data` structure containing the gas concentration reading.
     */
    MQ7Data gasRead(); ///< Member function declaration.

private:
    uint8_t gaspin; ///< Pin number used for interfacing with the MQ7 sensor.
};

#endif // MQ_7_SENSOR_H
