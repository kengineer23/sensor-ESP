/**
 * @file PMS5003Sensor.h
 * @brief Header file for the PMS5003Sensor class.
 * 
 * This header file defines the `PMS5003Sensor` class, which provides methods for interfacing
 * with the PMS5003 particulate matter sensor. The class includes functions for initializing
 * the sensor and reading data from it.
 */

#ifndef PMS_5003_SENSOR_H
#define PMS_5003_SENSOR_H

#include "PMS.h"

/**
 * @struct PMS5003Data
 * @brief Structure to hold PMS5003 sensor data.
 * 
 * This structure contains the measurement data from the PMS5003 sensor. Currently, it holds
 * the PM2.5 particulate matter concentration value.
 */
struct PMS5003Data {
    int pm2_5; ///< PM2.5 particulate matter concentration value.
};

/**
 * @class PMS5003Sensor
 * @brief A class for interfacing with the PMS5003 particulate matter sensor.
 * 
 * The `PMS5003Sensor` class provides methods to initialize the PMS5003 sensor and read
 * particulate matter data from it. The class uses the `PMS` library to communicate with
 * the sensor via a serial interface.
 */
class PMS5003Sensor {
    public:
        /**
         * @brief Constructs a PMS5003Sensor object.
         * 
         * The constructor initializes the `PMS5003Sensor` object and sets up the `PMS` instance
         * for communication. This sets up the necessary configuration for interfacing with the
         * PMS5003 sensor.
         */
        PMS5003Sensor(); ///< Constructor

        /**
         * @brief Initializes the PMS5003 sensor.
         * 
         * This method sets up the serial communication with the PMS5003 sensor and prepares
         * the sensor for operation by switching it to active mode and waking it up.
         */
        void begin(); ///< Initialize the PMS5003 sensor.

        /**
         * @brief Reads data from the PMS5003 sensor.
         * 
         * This method retrieves the PM2.5 particulate matter concentration from the PMS5003
         * sensor. It returns the data in a `PMS5003Data` structure and prints the PM2.5 value
         * to the serial monitor.
         * 
         * @return A `PMS5003Data` structure containing the PM2.5 concentration reading.
         */
        PMS5003Data readData(); ///< Read data from the PMS5003 sensor.

    private:
        PMS pms;       ///< PMS object for communication with the sensor.
        PMS::DATA data; ///< Variable to hold the data read from the sensor.
};

#endif  //!PMS_5003_SENSOR_H
