/**
 * @file PMS5003Sensor.cpp
 * @brief Implementation of the PMS5003Sensor class for interfacing with the PMS5003 sensor.
 * 
 * This file contains the implementation of the `PMS5003Sensor` class, which provides methods
 * for initializing and reading data from the PMS5003 particulate matter sensor. The class
 * uses the `Serial2` interface on the ESP32 for communication with the sensor.
 */

#include "PMS5003Sensor.h"
#include "Arduino.h"

/**
 * @brief Constructs a PMS5003Sensor object.
 * 
 * The constructor initializes the `PMS5003Sensor` object and sets up the `PMS5003` instance
 * to use `Serial2` for communication. This sets up the necessary UART channel on the ESP32.
 */
PMS5003Sensor::PMS5003Sensor() : pms(Serial2) {}

/**
 * @brief Initializes the PMS5003 sensor.
 * 
 * This method begins serial communication on `Serial2` with a baud rate of 9600. It also
 * switches the PMS5003 sensor to active mode and wakes it up for operation.
 */
void PMS5003Sensor::begin() {
    Serial2.begin(9600);    ///< Initialize UART2 with a baud rate of 9600.
    pms.activeMode();       ///< Switch the PMS5003 sensor to active mode.
    pms.wakeUp();           ///< Wake up the PMS5003 sensor.
}

/**
 * @brief Reads particulate matter data from the PMS5003 sensor.
 * 
 * This method retrieves data from the PMS5003 sensor. It reads the PM2.5 particulate matter
 * concentration from the sensor and stores it in a `PMS5003Data` structure. It also prints
 * the PM2.5 value to the serial monitor.
 * 
 * @return A `PMS5003Data` structure containing the PM2.5 concentration reading.
 */
PMS5003Data PMS5003Sensor::readData() {
    PMS5003Data pmsdata; ///< Structure to hold the PM2.5 data.
    if (pms.readUntil(data)) {
        pmsdata.pm2_5 = data.PM_AE_UG_2_5; ///< Store the PM2.5 reading.
        Serial.println(pmsdata.pm2_5);     ///< Print the PM2.5 reading to the serial monitor.
    }

    return pmsdata; ///< Return the data structure with the PM2.5 reading.
}
