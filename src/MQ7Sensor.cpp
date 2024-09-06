/**
 * @file MQ7Sensor.cpp
 * @brief Implementation of the MQ7Sensor class for interfacing with the MQ7 gas sensor.
 * 
 * This file contains the implementation of the `MQ7Sensor` class, which provides methods
 * for initializing and reading data from the MQ7 gas sensor.
 */

#include "MQ7Sensor.h"
#include "Arduino.h" // Include necessary library for Arduino functions

/**
 * @brief Constructs an MQ7Sensor object.
 * 
 * The constructor initializes the `MQ7Sensor` object by setting up the pin used for
 * interfacing with the MQ7 sensor. The pin is set as an input to read digital values
 * from the sensor.
 * 
 * @param pin The pin number to which the MQ7 sensor is connected.
 */
MQ7Sensor::MQ7Sensor(uint8_t pin) : gaspin(pin) {
    pinMode(gaspin, INPUT); ///< Initialize the pin mode for the sensor as INPUT.
}

/**
 * @brief Reads gas concentration data from the MQ7 sensor.
 * 
 * This method retrieves the gas concentration value from the MQ7 sensor. It reads a
 * digital value from the specified sensor pin and returns it in a `MQ7Data` structure.
 * 
 * @return A `MQ7Data` structure containing the gas concentration reading.
 */
MQ7Data MQ7Sensor::gasRead() {
    MQ7Data data; ///< Structure to hold the gas concentration data.
    data.gasValue = digitalRead(gaspin); ///< Read digital value from the sensor pin.
    return data; ///< Return the data structure with the gas concentration reading.
}
