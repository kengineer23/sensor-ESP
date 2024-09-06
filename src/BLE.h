/**
 * @file BLE.h
 * @brief Header file for BLE functionality.
 * 
 * This header file declares the BLE-related functions and classes used for Bluetooth
 * Low Energy (BLE) communication. It includes external variables, function declarations,
 * and class definitions for BLE server and characteristic callbacks.
 */

#ifndef BLE_H
#define BLE_H

#include <BLEDevice.h>
#include <BLEServer.h>

/**
 * @brief Indicates whether a BLE device is connected.
 * 
 * This external variable is set to `true` when a BLE device is connected and `false` otherwise.
 */
extern bool deviceConnected;

/**
 * @brief Indicates whether new data has been received via BLE.
 * 
 * This external variable is set to `true` when new data is received from a BLE client.
 */
extern bool newData;

/**
 * @brief Contains the received data from a BLE client.
 * 
 * This external variable holds the data received from a BLE client as a C-style string.
 */
extern const char* receivedData;

/**
 * @brief Initializes BLE functionality.
 * 
 * This function sets up BLE, including configuring services, characteristics, and starting
 * the BLE server.
 */
void setupBLE();

/**
 * @class MyServerCallbacks
 * @brief Callback class for handling BLE server events.
 * 
 * This class derives from `BLEServerCallbacks` and provides methods to handle BLE
 * server connection and disconnection events.
 */
class MyServerCallbacks: public BLEServerCallbacks {
    /**
     * @brief Called when a BLE device connects to the server.
     * 
     * This method is invoked when a BLE client establishes a connection with the BLE server.
     * 
     * @param pServer Pointer to the BLE server instance.
     */
    void onConnect(BLEServer* pServer);

    /**
     * @brief Called when a BLE device disconnects from the server.
     * 
     * This method is invoked when a BLE client disconnects from the BLE server.
     * 
     * @param pServer Pointer to the BLE server instance.
     */
    void onDisconnect(BLEServer* pServer);
};

/**
 * @class MyCallbacks
 * @brief Callback class for handling BLE characteristic events.
 * 
 * This class derives from `BLECharacteristicCallbacks` and provides a method to handle
 * write events on BLE characteristics.
 */
class MyCallbacks: public BLECharacteristicCallbacks {
    /**
     * @brief Called when a BLE client writes to a characteristic.
     * 
     * This method is invoked when a BLE client writes data to a characteristic on the server.
     * 
     * @param pCharacteristic Pointer to the BLE characteristic that was written to.
     */
    void onWrite(BLECharacteristic *pCharacteristic);
};

#endif // BLE_H
