#include "BLE.h"
#include <Arduino.h>
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

BLECharacteristic *pCharacteristic;
bool deviceConnected = false;
bool newData = false;
const char* receivedData = "";

void setupBLE() {
    // Initialize BLE
    BLEDevice::init("ISAAC");   // BLE device created
    BLEServer *pServer = BLEDevice::createServer();   // BLE Server created
    
    // Set a callback function to handle events triggered on the BLE Server
    pServer->setCallbacks(new MyServerCallbacks());

    BLEService *pService = pServer->createService(SERVICE_UUID);
    pCharacteristic = pService->createCharacteristic(
                        CHARACTERISTIC_UUID,
                        BLECharacteristic::PROPERTY_READ |
                        BLECharacteristic::PROPERTY_WRITE
                      );

    // Assign a callback function to handle events related to BLE Characteristic
    pCharacteristic->setCallbacks(new MyCallbacks());
    pCharacteristic->setValue("Hello World");

    pService->start();

    // Manage advertising settings of BLE device
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(true);

    // Sets the advertising interval in hexadecimal format
    pAdvertising->setMinPreferred(0x06);  // 3.75 ms
    pAdvertising->setMinPreferred(0x12);  // 11.25 ms
    BLEDevice::startAdvertising();
}

void MyServerCallbacks::onConnect(BLEServer* pServer) {
    deviceConnected = true;
}

void MyServerCallbacks::onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
}

void MyCallbacks::onWrite(BLECharacteristic *pCharacteristic) {
    receivedData = pCharacteristic->getValue().c_str(); // Convert to const char*
    Serial.println(receivedData);
    newData = true;
}
